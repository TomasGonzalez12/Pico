//Indicadores
#define LED1_PIN        (17)
#define LED2_PIN        (16)

//Pulsadores
#define PULS1_PIN       (15)
#define PULS2_PIN       (12)
#define DEMORA          (30)
#define ON              (0) // si pusimos pull up, si no, dar vuelta
#define OFF             (1)

// Configuración de UART1 (id de uart, irq de uart y baudrate)
#define UART_ID         (uart1)
#define UART_IRQ_ID     (UART1_IRQ)
#define BAUD_RATE       (115200)

#define UART_TX_PIN     (4)
#define UART_RX_PIN     (5)
#define BYTE_STX        (0x02)
#define BYTE_ETX        (0x03)

//BUZZER
#define BUZZ_PIN        (18)

#define F_PWM           (1000) //KHz
#define DUTY            (0.5f)
#define F_COUNTER       (150000000UL)
#define WRAP            (4095) 
#define LEVEL           (WRAP/2)
#define DIV             ((float) F_COUNTER/(F_PWM * (WRAP + 1)))

//ADC BATERIA
#define BAT_5V          (26) //ADC0 Tanque
#define BAT_12V         (27) // ADC1 Cisterna
#define MUESTRAS        (10) // por si hacemos promedio de muestras