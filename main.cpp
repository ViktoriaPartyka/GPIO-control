#include <iostream>
#include <string>
#include <cmath>
#include <chrono>
#include <thread>
#include <gpiod.h>

using namespace std;

#define PWM_PERIOD_NS 1000000 // 1 ms period for PWM
#define MAX_DUTY_CYCLE 100 // maximum duty cycle percentage

int calculateDutyCycle(int brightness) 
{
    return (brightness * PWM_PERIOD_NS) / MAX_DUTY_CYCLE;
} 

void toggleGpioLine(gpiod_line* led_line, int duty_cycle_ns) 
{
    int ret = gpiod_line_set_value(led_line, 1);
    if (ret < 0) 
    {
        std::cerr << "Failed to set GPIO line value" << std::endl;
        exit(1);
    }
    std::this_thread::sleep_for(std::chrono::nanoseconds(duty_cycle_ns));

    ret = gpiod_line_set_value(led_line, 0);
    if (ret < 0) 
    {
        std::cerr << "Failed to set GPIO line value" << std::endl;
        exit(1);
    }
    std::this_thread::sleep_for(std::chrono::nanoseconds(PWM_PERIOD_NS - duty_cycle_ns));
}

void waveOne(gpiod_line* led_line, int brightness)
{
    while(true) 
    {
        int duty_cycle_ns = calculateDutyCycle(brightness);
        
        for(int i = 0; i < 1000; i++)
        {
            toggleGpioLine(led_line, duty_cycle_ns);
        }
            brightness += 10;

 	            if(brightness > 100)
                {
                    brightness = 0;
                }
    }
}

void waveTwo(gpiod_line* led_line, int brightness)
{
    while(true)
    {
        while(brightness != 100)
        {
            int duty_cycle_ns = calculateDutyCycle(brightness);
            // Fade in
            for (int i = 0; i < 100; i++) 
            {
                toggleGpioLine(led_line, duty_cycle_ns);
            }  
            brightness += 5;
        }
        while(brightness != 0)
        {
	        int duty_cycle_ns = calculateDutyCycle(brightness);
            for (int i = 100; i >= 0;i--) 
            {
                toggleGpioLine(led_line, duty_cycle_ns); 
            }
            brightness -= 5;
        }
    }
}

void waveThree(gpiod_line* led_line, int brightness)
{
    while(true) 
    {
        int duty_cycle_ns = calculateDutyCycle(brightness);
        
        for(int i = 0; i < 1000; i++)
        {
            toggleGpioLine(led_line, duty_cycle_ns);
        }

 	    if(brightness == 100)
        {
            brightness = 0;
        }
        else
        {
            brightness = 100;
        }
    }
}

int main(int argc, char* argv[]) 
{
    if (argc != 5) 
    {
        std::cerr << "Usage: " << argv[0] << " -b <percentage> -p <pattern>" << std::endl;
        return 1;
    }

    // Parse command line arguments
    int brightness = stoi(argv[2]);
    if (brightness < 0 || brightness > 100) 
    {
        std::cerr << "Brightness percentage must be between 0 and 100" << std::endl;
        return 1;
    }
    
    // Open the GPIO chip
    gpiod_chip* chip = gpiod_chip_open("/dev/gpiochip0");
    if (!chip) 
    {
        std::cerr << "Failed to open GPIO chip" << std::endl;
        return 1;
    }

    // Open the GPIO line for the LED
    int led_line_num = 23; // GPIO pin 23
    gpiod_line* led_line = gpiod_chip_get_line(chip, led_line_num);
    if (!led_line) 
    {
        std::cerr << "Failed to get GPIO line" << std::endl;
        return 1;
    }

    // Request the GPIO line for output
    int ret = gpiod_line_request_output(led_line, "LED", 0);
    if (ret < 0) 
    {
        std::cerr << "Failed to request GPIO line for output" << std::endl;
        return 1;
    }

   string pattern = (argv[4]);

 if (pattern == "1") 
 {
    waveOne(led_line,brightness);
 } 
 else if (pattern == "2")
 {
    waveTwo(led_line,brightness);
 }
 else if (pattern == "3") 
 {
    waveThree(led_line,brightness);
 }
    // Release the GPIO line
    gpiod_line_release(led_line);

    // Close the GPIO chip
    gpiod_chip_close(chip);

    return 0;
}
