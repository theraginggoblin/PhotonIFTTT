// Set your timezone here
#define TIMEZONE 10.5

// Identify analog pin for reading lux
int photoresistor = A0;

// Declaration of lux variable to be used in loop
int lux = 0;

// bool to track status of sunlight. False means light is too low, true means appropriate
bool sunlight = false;

// Somewhere to store how many minutes of sunshine our plant has had
int sunlight_duration = 0;

// Bool to identify when we've had 2 hours of sunlight for the day
bool two_hours_sunlight = false;

// If lux below threshold sunlight = false, if lux above threshold sunlight = true. Notifies on change

void lux_handle(int lux) {
    if (lux < 100 && sunlight == true) {
        sunlight = false;
        Particle.publish("lux_changed", "not suitable");
    }
    
    if (lux > 400 && sunlight == false) {
        sunlight = true;
        Particle.publish("lux_changed", "appropriate");
    }
}

// counts sunlight up to 120 (lines up with our delay in loop), then notifies if 2 hours of sunlight reached

void sunlight_count() {
    if (sunlight == true) {
        sunlight_duration += 1;
        
        if (sunlight_duration == 120 && two_hours_sunlight == false) {
            Particle.publish("sunlight_reached");
            two_hours_sunlight = true;
        }
    }
}

// Reset the count every night ahead of the new day

void sunlight_reset() {
    if (Time.hour() == 0 && sunlight_duration > 0) {
        two_hours_sunlight = false;
        sunlight_duration = 0;
    }
}

// Configure photoresister pin as input & configure timezone

void setup() {
    pinMode(photoresistor, INPUT);
    Time.zone(TIMEZONE);
}

void loop() {
    lux = analogRead(photoresistor);
    
    lux_handle(lux);

    delay(60000);

    sunlight_count();
}
