#ifndef RSCAN_DOSE_CLIENT_H
#define RSCAN_DOSE_CLIENT_H

#include "tcp_client.h"
#include <vector>

enum class RSCANDoseSensitivity
{
    Low = 1,
    High = 0
};

enum class RSCANDoseVoltagePolarity {
    Negative = 0,
    Positive = 1
};

// command list and sequence as it is in MCU program
enum RSCANDoseCommandCode
{
    Empty = 0x00000000l,
    Enable_HV_A = 0x01,
    Enable_HV_B = 0x02,
    Disable_HV_A = 0x03,
    Disable_HV_B = 0x04,
    Set_Voltage_A = 0x05,
    Set_Voltage_B = 0x06,
    Set_HV_Polarity_A = 0x07,
    Set_HV_Polarity_B = 0x08,
    Set_Sensitivity_A = 0x09,
    Set_Sensitivity_B = 0x0A,

    Set_ADC_DR_A = 0x40,
    Set_ADC_DR_B = 0x41,
    Set_ADC_Gain_A = 0x42,
    Set_ADC_Gain_B = 0x43
};

// value list and sequence as it is in MCU program
enum RSCANDoseValueCode
{
   	Message_Num,
	Dose_A,
	Dose_B,
	HV_A,
	HV_B,
	Temp_A,
	Temp_B,
    Sensitivity_A,
    Sensitivity_B,
	HV_Polarity_A,
	HV_Polarity_B,
    HV_Enabled_A,
    HV_Enabled_B,
    Version_Major,
    Version_Minor,
	Size 
};

class RSCANDoseClient : public TCPClient
{
    static const int kRxBufferSize = 64;
    static const int kTxBufferSize = 64;

    struct ValueAddress
    {
        uint32_t offset;
        uint32_t size;
    };

    std::vector<ValueAddress> map;
    uint8_t tx[kTxBufferSize];

public:
    RSCANDoseClient(std::string ip, uint16_t port);
    ~RSCANDoseClient();
    void sendCommand(RSCANDoseCommandCode command, int parameter = 0);
    int readValue(RSCANDoseValueCode valueCode);
};

#endif
