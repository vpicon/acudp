# AssettoCorsa UDP Interface
The document helps you to set up UDP socket comunication between your application 
and Assetto Corsa. 

The PC running Assetto Corsa will be referred as the *ACServer*. Also the following
are assumed:
- `int` are 32 bit little-endian integers
- `float` are 32 bit floating point numbers
- `bool` are 8 bit boolean value

## 1. Connection Setup to AC via UDP: Handshake
### 1.1. Client starts comunication with AC Remote Telemetry Server

The handshake process to connect and receive Remote Telemtry data from Assetto 
Corsa via UDP works as follows: your app first must create an UDP socket and 
connect to a PC address running Assetto Corsa (ACServer). The connection port 
number is **9996**.

Your application must send a structured data using the following format:

```c
struct setup {
    int identifier;
    int version;
    int operationId;
};
```

The fields work as follows:

+ `int identifier`: (not used in the current Remote Telemtry version by AC) set it to `1`.
+ `int version`: (not used in the current Remote Telemtry version by AC) set it to `1`.
+ `int operationId`: This is the type of operation required by the client. The following operations are available:
⋅⋅-`HANDSHAKE = 0`: This operation identifier must be set when the client wants to start the comunication.
⋅⋅-`SUBSCRIBE_UPDATE = 1`: This operation identifier must be set when the client wants to be updated from the specific ACServer.
⋅⋅-`SUBSCRIBE_SPOT = 2`: This operation identifier must be set when the client wants to be updated from the specific ACServer just for SPOT Events (e.g.: the end of a lap).
⋅⋅-`DISMISS = 3`: This operation identifier must be set when the client wants to leave the comunication with ACServer.

In summary, for the first handshaking phase your application will need to send the following structured data to ACServer

```c
struct setup {
    .identifier = 1,
    .version = 1,
    .operationId= 0
};
```

### 1.2. AC Remote Telemetry Server Responds to the client
After sending the structured data in section 1.1, your application will receive the following struct as response:

```c
struct setupResponse {
    char carName[50];
    char driverName[50];
    int identifier;
    int version;
    char trackName[50];
    char trackConfig[50];
};
```

Each of the fields:

-`char carName[50]`: is the name of the car that the player is driving on the AC Server,
-`char driverName[50]`:is the name of the driver running on the AC Server,
-`int identifier`: for now is just `4242`,
-`int version`: for now is set to `1`, this will identify the version running on the AC Server,
-`char trackName[50]`:is the name of the track on the AC Server,
-`char trackConfig[50]`: is the track configuration.

This data is necessary to understand which driver are we connecting to.

### 1.3. AC Client confirms connection
Again the client must send the following structured data, the same from section 1.1 :

```c
struct setup {
    int identifier;
    int version;
    int operationId;
};
```

Now operationId must be one either `SUBSCRIBE_UPDATE = 1`, this operation identifier must be set when the client wants to be updated from the specific ACServer; or `SUBSCRIBE_SPOT = 2` this operation identifier must be set when the client wants to be updated from the specific ACServer just for SPOT Events (e.g.: the end of a lap).

After this phase the Client is added as a listener to AC Remote Telemetry listeners.


## 2. ACServer updating clients
### 2.1 Update Events
For each physics step, ACServer will call the update function to all the listeners. If the client subscribed himself with `SUBSCRIBE_UPDATE` identifier, it will receive the following structured data:

```c
struct RTCarInfo {
    char identifier;
    int size;

    float speed_Kmh;
    float speed_Mph;
    float speed_Ms;

    bool isAbsEnabled;
    bool isAbsInAction;
    bool isTcInAction;
    bool isTcEnabled;
    bool isInPit;
    bool isEngineLimiterOn;


    float accG_vertical;
    float accG_horizontal;
    float accG_frontal;

    int lapTime;
    int lastLap;
    int bestLap;
    int lapCount;

    float gas;
    float brake;
    float clutch;
    float engineRPM;
    float steer;
    int gear;
    float cgHeight;

    float wheelAngularSpeed[4];
    float slipAngle[4];
    float slipAngle_ContactPatch[4];
    float slipRatio[4];
    float tyreSlip[4];
    float ndSlip[4];
    float load[4];
    float Dy[4];
    float Mz[4];
    float tyreDirtyLevel[4];

    float camberRAD[4];
    float tyreRadius[4];
    float tyreLoadedRadius[4];
    float suspensionHeight[4];

    float carPositionNormalized;
    float carSlope;
    float carCoordinates[3];
};
```

Some clarifications on the fields:
-`char identifier` is set to char `'a'`, it is used to understand that the structured data is the data that the client app wants
-`int size`: the size of the structured data in bytes.

### 2.1 Spot Events
If the client subscribed himself with `SUBSCRIBE_SPOT` identifier, it will receive the following structured data whenever a spot event is triggered (for example for the end of a lap). Differently from `SUBSCRIBE_UPDATE`, this event will interest all the cars in the AC session:

```c
struct RTLap {
    int carIdentifierNumber;
    int lap;
    char driverName[50];
    char carName[50];
    int time_millis;
};
```

## 3. Dismissing an AC Client
A client to dimiss himself he must send the following package (the same from session 1.1)

```c
struct setup dismiss_setup {
    .identifier=1
    .version=1
    .operationId=3
};
```

Now setting `DISMISS = 3` as `operationId`. The client will be removed from the listeners, ACServer will forget about him and no more updates will be sent to him. To connect again, do again the steps from section 1.1 or 1.2.
