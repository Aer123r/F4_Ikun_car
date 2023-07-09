#include "common_inc.h"
#include "freertos_inc.h"

#include "ArduinoJson.h"
#include "string"

Motor motor;
Encoder encoder1(&htim1);
Encoder encoder2(&htim3);
Encoder encoder3(&htim4);
Encoder encoder4(&htim8);
MPU6050 mpu6050(&hi2c1);

Servo servo(&htim9,TIM_CHANNEL_1);

StaticJsonDocument<200> jsonDocument;
uint8_t rx_data[100];

float pwm;

/* LED Blinking Task */
void LedBlinkyTask(void const *argument) {

    encoder1.Start();
    std::string s;
    while (1) {
        osMutexWait(pwmMutexHandle,10);
        s = std::to_string(pwm);
        osMutexRelease(pwmMutexHandle);
        HAL_UART_Transmit_IT(&huart1,(uint8_t *)s.c_str(),s.size());
        HAL_GPIO_TogglePin(GPIOF, GPIO_PIN_9);
        osDelay(1000);
    }
}
/**
 * @def 舵机控制任务
 * @param argument
 */

void ServoHandleTask(void const *argument) {
    servo.Init();
    servo.SetAngle(210);
    while(1){
        osDelay(200);
    }
}


/**
 * TODO 处理编码器数据和更加PWM
 * @def 电机控制任务
 */

void MotorHandleTask(void const *argument) {
//    static int64_t prev_count = encoder1.GetCount();
//    motor.Move();
//    osDelay(5000);
    motor.Stop();
    while (1) {
        osMutexWait(pwmMutexHandle,10);
//        int64_t count = encoder1.GetCount();
//        if(count >= prev_count){
//            pwm = count - prev_count;
//        }else{
//            pwm = count+65535-prev_count;
//        }
//        pwm = pwm/550;
//        prev_count = count;
        osMutexRelease(pwmMutexHandle);
        osDelay(50);
    }
}

/**
 * @name 障碍物识别处理任务
 * @param argument
 * @interface UART
 */
void obstacleDetectionAndProcessingTask(void const *argument) {
    while(1) {
        HAL_UARTEx_ReceiveToIdle_IT(&huart1,rx_data,100);
        osDelay(50);
    }
}
void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size){
    if(huart->Instance == USART1){
        //解析数据
        DeserializationError error = deserializeJson(jsonDocument, rx_data);
        if (!error) {
            const char *myC = jsonDocument["myChar"];
            HAL_UART_Transmit_IT(&huart1,(uint8_t*)myC,strlen(myC));
        }
    }
}
/**
 * @name 光电传感器任务
 * @param 触发的IO号
 * @details IO在上升沿和下降沿会触发这个函数
 *
 */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin){
    if(GPIO_Pin == GPIO_PIN_2){
    }else{}
}

/* 主函数 */
void Main() {

//    do
//    {
//        mpu6050.Init();
//        osDelay(100);
//    } while (!mpu6050.testConnection());
//    mpu6050.InitFilter(200, 100, 50);
    /* 初始化 */
    motor.Init();

    osMutexDef(pwmMutex);
    pwmMutexHandle = osMutexCreate(osMutex(pwmMutex));

    osThreadDef(LedBlinkyTask_, LedBlinkyTask, osPriorityNormal, 0, 256);
    ledBlinkyTaskHandle = osThreadCreate(osThread(LedBlinkyTask_), NULL);

    osThreadDef(MotorTask, MotorHandleTask, osPriorityNormal, 0, 512);
    motorTaskHandle = osThreadCreate(osThread(MotorTask), NULL);

    osThreadDef(ServoTask, ServoHandleTask, osPriorityAboveNormal, 0, 512);
    servoTaskHandle = osThreadCreate(osThread(ServoTask), NULL);

    osThreadDef(ObstacleDetectionTask,obstacleDetectionAndProcessingTask,osPriorityHigh,0,512);
    obstacleTaskHandle = osThreadCreate(osThread(ObstacleDetectionTask), NULL);
}
