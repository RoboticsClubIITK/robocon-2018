/*
#include <ros.h>
#include <Arduino.h>
#include <std_msgs/Float64.h>
#include <math.h>

int flSpeed = 3;
int flDirn1 = 24;
int flDirn2 = 25;
int frSpeed = 2;
int frDirn1 = 23;
int frDirn2 = 22;
int blSpeed = 4;
int blDirn1 = 27;
int blDirn2 = 26;
int brSpeed = 5;
int brDirn1 = 29;
int brDirn2 = 28;

float vd=0.75;

int fl=205;
int bl=190;
int fr=66;//66//65
int br=220;//255//250
int flMotorSpeed,blMotorSpeed,frMotorSpeed,brMotorSpeed;

ros::NodeHandle nh;

void angle_callback(const std_msgs::Float64 &theta)
{
    float angle = theta.data;
    //nh.loginfo("Angle: %f", val);

    flMotorSpeed = fl*vd*sin( angle + 0.785);
    blMotorSpeed = bl*vd*cos( angle + 0.785);
    frMotorSpeed = fr*vd*cos( angle + 0.785);
    brMotorSpeed = br*vd*sin( angle + 0.785);

    digitalWrite(flDirn1, HIGH);
    digitalWrite(flDirn2, LOW);
    analogWrite(flSpeed, flMotorSpeed);

    digitalWrite(blDirn1, HIGH);
    digitalWrite(blDirn2, LOW);
    analogWrite(blSpeed, blMotorSpeed);   
   
    digitalWrite(frDirn1, HIGH);
    digitalWrite(frDirn2, LOW);
    analogWrite(frSpeed, frMotorSpeed);
    
    digitalWrite(brDirn1, HIGH);
    digitalWrite(brDirn2, LOW);
    analogWrite(brSpeed, brMotorSpeed);
}

ros::Subscriber<std_msgs::Float64> sub_angle("/robocon2018/angle", &angle_callback);

void setup()
{  
    pinMode(flDirn1, OUTPUT);
    pinMode(flDirn2, OUTPUT);
    pinMode(flSpeed, OUTPUT);
    pinMode(frDirn1, OUTPUT);
    pinMode(frDirn2, OUTPUT);
    pinMode(frSpeed, OUTPUT);
    pinMode(blDirn1, OUTPUT);
    pinMode(blDirn2, OUTPUT);
    pinMode(blSpeed, OUTPUT);
    pinMode(brDirn1, OUTPUT);
    pinMode(brDirn2, OUTPUT);
    pinMode(brSpeed, OUTPUT);

    nh.initNode();
    nh.subscribe(sub_angle);
}

void loop() 
{ 
    nh.spinOnce();
}
*/













#include <ros.h>
#include <Arduino.h>
#include <std_msgs/Float64.h>
#include <math.h>

#define FR 0.66
#define FL 2.05
#define BR 2.20
#define BL 1.90

float Kp=0.25;
float Ki=0.0;
float Kd=0.0;

int maxRPM= 255;
int baseRPM=70;

#define flSpeed  3
#define flDirn1  24
#define flDirn2  25
#define frSpeed  2
#define frDirn1  23
#define frDirn2  22
#define blSpeed  4
#define blDirn1  27
#define blDirn2  26
#define brSpeed  5
#define brDirn1  29
#define brDirn2  28

float angle;
float distance;
int error=0;
int lastError=0;

ros::NodeHandle nh;

int limRPM(int rpm)
{
    if (rpm>maxRPM)
    {
        return maxRPM;
    }
    return rpm;
}

void reset()
{
    digitalWrite(flDirn1, LOW);
    digitalWrite(flDirn2, LOW);
    digitalWrite(frDirn1, LOW);
    digitalWrite(frDirn2, LOW);
    analogWrite(flSpeed, 0);
    analogWrite(frSpeed, 0);
    digitalWrite(blDirn1, LOW);
    digitalWrite(blDirn2, LOW);
    digitalWrite(brDirn1, LOW);
    digitalWrite(brDirn2, LOW);
    analogWrite(blSpeed, 0);
    analogWrite(brSpeed, 0);
}

void move(float rpmr,float rpml)
{
    digitalWrite(flDirn1, HIGH);
    digitalWrite(flDirn2, LOW);
    digitalWrite(frDirn1, HIGH);
    digitalWrite(frDirn2, LOW);
    analogWrite(flSpeed, limRPM((int)(FL*rpml)));
    analogWrite(frSpeed, limRPM((int)(FR*rpmr)));
    digitalWrite(blDirn1, HIGH);
    digitalWrite(blDirn2, LOW);
    digitalWrite(brDirn1, HIGH);
    digitalWrite(brDirn2, LOW);
    analogWrite(blSpeed, limRPM((int)(BL*rpml)));
    analogWrite(brSpeed, limRPM((int)(BR*rpmr)));
}

void sideway_right(float rpmr,float rpml)
{
    digitalWrite(flDirn1, HIGH);
    digitalWrite(flDirn2, LOW);
    digitalWrite(frDirn1, LOW);
    digitalWrite(frDirn2, HIGH);
    analogWrite(flSpeed, limRPM((int)(FL*rpml)));
    analogWrite(frSpeed, limRPM((int)(FR*rpmr)));
    digitalWrite(blDirn1, LOW);
    digitalWrite(blDirn2, HIGH);
    digitalWrite(brDirn1, HIGH);
    digitalWrite(brDirn2, LOW);
    analogWrite(blSpeed, limRPM((int)(BL*rpml)));
    analogWrite(brSpeed, limRPM((int)(BR*rpmr)));
}

void sideway_left(float rpmr,float rpml)
{
    digitalWrite(flDirn1, LOW);
    digitalWrite(flDirn2, HIGH);
    digitalWrite(frDirn1, HIGH);
    digitalWrite(frDirn2, LOW);
    analogWrite(flSpeed, limRPM((int)(FL*rpml)));
    analogWrite(frSpeed, limRPM((int)(FR*rpmr)));
    digitalWrite(blDirn1, HIGH);
    digitalWrite(blDirn2, LOW);
    digitalWrite(brDirn1, LOW);
    digitalWrite(brDirn2, HIGH);
    analogWrite(blSpeed, limRPM((int)(BL*rpml)));
    analogWrite(brSpeed, limRPM((int)(BR*rpmr)));
}

/*void distance_callback(const std_msgs::Float64 &dist)
{
    float distance = dist.data;
    //nh.loginfo("Dist: %f",val);

    int distlim = 10;
    float rpmr=2,rpml=2;
    //if(distance>50)
    //{
    if (distance < -distlim)
    {
        sideway_right(rpmr,rpml);
        nh.loginfo("sideway right...");
    }

    else if (distance > distlim)
    {
        sideway_left(rpmr,rpml);
        nh.loginfo("sideway left...");
    }
    else
    {
        straight(rpmr,rpml);
    }
    //}
}*/

//void angle_callback(const std_msgs::Float64 &theta)

    //float angle = theta.data;
    //nh.loginfo("Angle: %f",val);


void distance_callback(const std_msgs::Float64 &dist)
{
    float distance = dist.data;
    error=-distance;

    float rpm = Kp * error + Kd * (error - lastError) + Ki*( error + lastError );
    lastError = error;

    float rpmr=baseRPM-rpm;
    float rpml=baseRPM+rpm;

    move(rpmr,rpml);
}

//ros::Subscriber<std_msgs::Float64> sub_angle("/robocon2018/angle", &angle_callback);
ros::Subscriber<std_msgs::Float64> sub_distance("/robocon2018/distance", &distance_callback);


void setup()
{
    // put your setup code here, to run once:

    pinMode(flDirn1, OUTPUT);
    pinMode(flDirn2, OUTPUT);
    pinMode(flSpeed, OUTPUT);
    pinMode(frDirn1, OUTPUT);
    pinMode(frDirn2, OUTPUT);
    pinMode(frSpeed, OUTPUT);
    pinMode(blDirn1, OUTPUT);
    pinMode(blDirn2, OUTPUT);
    pinMode(blSpeed, OUTPUT);
    pinMode(brDirn1, OUTPUT);
    pinMode(brDirn2, OUTPUT);
    pinMode(brSpeed, OUTPUT);

    nh.initNode();
    //nh.subscribe(sub_angle);
    
    nh.subscribe(sub_distance);
    //nh.subcribe(sub_rotcon);
}

void loop()
{
    nh.spinOnce();
}