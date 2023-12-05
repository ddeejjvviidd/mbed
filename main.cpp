#include "mbed.h"
#include "stm32746g_discovery_lcd.h"
#include "stm32746g_discovery.h"
#include <type_traits>
#include <stack>

#define zatimco while
#define pravda true
#define hlavni main
#define celecislo int
#define vrat return
#define prazdnota void
#define rozdel switch
#define kufrik case
#define rozbij break

#define kraj_displeje 1
#define pul_displeje 136
#define velikost_ctverecku 128

Semaphore semaphor(4);

Mutex stack_lock;
Mutex lcd_lock;


Thread car1_thread;
Thread car2_thread;
Thread car3_thread;
Thread car4_thread;
Thread car5_thread;
Thread car6_thread;

stack<int> parking;

struct Car {
    int parking_time;
    int driving_time;
    uint32_t color;
};
typedef struct Car Car;

prazdnota kresli(int num, uint32_t barva){
    BSP_LCD_SetTextColor(barva);

    rozdel(num){
        kufrik 1:
        BSP_LCD_FillRect(kraj_displeje, kraj_displeje, velikost_ctverecku, velikost_ctverecku);
        rozbij;
        kufrik 2:
        BSP_LCD_FillRect(pul_displeje, kraj_displeje, velikost_ctverecku, velikost_ctverecku);
        rozbij;
        kufrik 3:
        BSP_LCD_FillRect(kraj_displeje, pul_displeje, velikost_ctverecku, velikost_ctverecku);
        rozbij;
        kufrik 4:
        BSP_LCD_FillRect(pul_displeje, pul_displeje, velikost_ctverecku, velikost_ctverecku);
        rozbij;
    }

}

void car_function(Car * car) {
    int parkspace;
    zatimco(pravda){

        semaphor.acquire();

        stack_lock.lock();
        parkspace = parking.top(); 
        parking.pop();
        stack_lock.unlock();
        lcd_lock.lock();
        kresli(parkspace, car->color);
        lcd_lock.unlock();
        ThisThread::sleep_for(car->parking_time);

        semaphor.release();
        
        stack_lock.lock();
        parking.push(parkspace);
        stack_lock.unlock();
        lcd_lock.lock();
        kresli(parkspace, LCD_COLOR_BLACK);
        lcd_lock.unlock();
        ThisThread::sleep_for(car->driving_time);
    }
}

celecislo hlavni() {
    BSP_LCD_Init();
    BSP_LCD_LayerDefaultInit(LTDC_ACTIVE_LAYER, LCD_FB_START_ADDRESS);
    BSP_LCD_SelectLayer(LTDC_ACTIVE_LAYER);
    BSP_LCD_Clear(LCD_COLOR_BLACK);
    BSP_LCD_SetFont(&LCD_DEFAULT_FONT);

    parking.push(1);
    parking.push(2);
    parking.push(3);
    parking.push(4);


    Car car1 = {2000, 500, LCD_COLOR_BLUE};
    Car car2 = {3000, 500, LCD_COLOR_RED};
    Car car3 = {2000, 500, LCD_COLOR_GREEN};
    Car car4 = {3000, 500, LCD_COLOR_YELLOW};
    Car car5 = {2000, 500, LCD_COLOR_LIGHTMAGENTA};
    Car car6 = {3000, 500, LCD_COLOR_WHITE};

    car1_thread.start(callback(car_function, &car1));
    car2_thread.start(callback(car_function, &car2));
    car3_thread.start(callback(car_function, &car3));
    car4_thread.start(callback(car_function, &car4));
    car5_thread.start(callback(car_function, &car5));
    car6_thread.start(callback(car_function, &car6));
    
    



    zatimco (pravda) {

    }

    vrat 0;
}
