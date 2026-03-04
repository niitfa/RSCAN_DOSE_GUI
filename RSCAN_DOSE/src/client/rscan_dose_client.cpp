#include "rscan_dose_client.h"
#include <string.h>
#include <iostream>

RSCANDoseClient::RSCANDoseClient(std::string ip, uint16_t port)
    : TCPClient(ip, port, kRxBufferSize, kTxBufferSize)
{
    // value sizes and offsets as it is in MCU program
    map = std::vector<ValueAddress>(RSCANDoseValueCode::Size);
    map[RSCANDoseValueCode::Message_Num]   = {0, 4};
    map[RSCANDoseValueCode::Dose_A]        = {4, 4};
    map[RSCANDoseValueCode::Dose_B]        = {8, 4};
    map[RSCANDoseValueCode::HV_A]          = {12, 4};
    map[RSCANDoseValueCode::HV_B]          = {16, 4};
    map[RSCANDoseValueCode::Temp_A]        = {20, 4};
    map[RSCANDoseValueCode::Temp_B]        = {24, 4};
    map[RSCANDoseValueCode::Range_A]       = {28, 1};
    map[RSCANDoseValueCode::Range_B]       = {29, 1};
    map[RSCANDoseValueCode::HV_Polarity_A] = {30, 1};
    map[RSCANDoseValueCode::HV_Polarity_B] = {31, 1};
}

RSCANDoseClient::~RSCANDoseClient()
{}

void RSCANDoseClient::sendCommand(RSCANDoseCommandCode command, int parameter)
{
    memset(this->tx, 0, this->kTxBufferSize);
    memcpy(this->tx + 0, &command, 4); // copy command
    memcpy(this->tx + 4, &parameter, 4); // copy parameter
    TCPClient::copyToTx(this->tx, 0, this->kTxBufferSize);
}

int RSCANDoseClient::readValue(RSCANDoseValueCode valueCode)
{
    int res = 0;
    if(valueCode < RSCANDoseValueCode::Size)
    {
        TCPClient::copyFromRx(&res, map[valueCode].offset, map[valueCode].size);
    }
    return res;
}

