#include "Particle.h"

int state;

#define MINUTE 10000


unsigned long timeNow; 
int mins;
int ambientLight;

void setup()
{
    mins=0;
    timeNow=0;
    state = 0;
    ambientLight = random(15,30); // we state with a initial reading 
 

    Serial.begin(9600); 
}

void loop()
{
   switch(state)
   {
        case 0:
            if(ambientLight>=17) // if ambient light is equal or above our set threshold
            {
              //  Serial.printf("Optimal light Reached changing State to 1\n"); 
                Particle.publish("Recevied Optimal Light", PUBLIC); // we trigger our event and IFTTT sends a email notification
                timeNow = millis();
                state = 1; // move to state 1
            }
            else
            {
               // Serial.printf("Currently in state 0, waiting for right amount of light\n");
                ambientLight = random(15,30);  // we take another sensor reading if dont yet meet the optimal light level.
                delay(5*MINUTE); // we take readings every 5 minutes.
            }
            break;
            
        case 1:
            
            if(millis() - timeNow >= 5*MINUTE) //  check if 5 minutes have passed
            {
                timeNow = millis();
                mins+=5;
              //  Serial.printf("5 min has passed with right amount of light\n");
                ambientLight = random(15,30); // take another sensor reading
                if(ambientLight <= 16) // if we fall below the threshold move back to state 0
                {
                   //  Particle.publish("Not Receving Optimal Light", String(ambientLight), PRIVATE);  we can send a notification if we fall back a state but i am not implementing this as i dont want so many notifications.
                    state=0;
                 
                    
                }
                else if(mins>=120)
                {
                    Particle.publish("Done For Today", String(ambientLight), PRIVATE); 
                    
                 
                    state = 2;
                }
                
            }
        
            break;
            
        case 2:
            
            if(millis() - timeNow >= 30*MINUTE)
            {
                timeNow = millis();
                 Particle.publish("Done For Today", String(ambientLight), PRIVATE);
            }
            break;
   }
}





