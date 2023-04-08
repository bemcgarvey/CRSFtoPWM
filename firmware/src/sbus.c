#include "definitions.h"
#include "sbus.h"
#include "rxTask.h"
#include "settings.h"

#define SBUS_HEADER     0x0f
#define SBUS_FOOTER     0x00
#define SBUS_FAILSAFE   0x08

typedef struct __attribute__((packed)) {
    uint8_t header;
    uint8_t channels[22];
    uint8_t flags;
    uint8_t footer;
}
SBusPacket;

static volatile __attribute__((aligned(4))) SBusPacket sbusPacket;

void initSBus(void) {
    //initialize packet;
    for (int i = 0; i < 22; ++i) {
        sbusPacket.channels[i] = 0;
    }
    sbusPacket.flags = 0;
    sbusPacket.header = SBUS_HEADER;
    sbusPacket.footer = SBUS_FOOTER;
}

void sendSBusPacket(void) {
    sbusPacket.channels[0] = channels[0];
    sbusPacket.channels[1] = (channels[0] >> 8) | (channels[1] << 3);
    sbusPacket.channels[2] = (channels[1] >> 5) | (channels[2] << 6);
    sbusPacket.channels[3] = (channels[2] >> 2);
    sbusPacket.channels[4] = (channels[2] >> 10) | (channels[3] << 1);
    sbusPacket.channels[5] = (channels[3] >> 7) | (channels[4] << 4);
    sbusPacket.channels[6] = (channels[4] >> 4) | (channels[5] << 7);
    sbusPacket.channels[7] = (channels[5] >> 1);
    sbusPacket.channels[8] = (channels[5] >> 9) | (channels[6] << 2);
    sbusPacket.channels[9] = (channels[6] >> 6) | (channels[7] << 5);
    sbusPacket.channels[10] = (channels[7] >> 3);
    sbusPacket.channels[11] = channels[8];
    sbusPacket.channels[12] = (channels[8] >> 8) | (channels[9] << 3);
    sbusPacket.channels[13] = (channels[9] >> 5) | (channels[10] << 6);
    sbusPacket.channels[14] = (channels[10] >> 2);
    sbusPacket.channels[15] = (channels[10] >> 10) | (channels[11] << 1);
    sbusPacket.channels[16] = (channels[11] >> 7) | (channels[12] << 4);
    sbusPacket.channels[17] = (channels[12] >> 4) | (channels[13] << 7);
    sbusPacket.channels[18] = (channels[13] >> 1);
    sbusPacket.channels[19] = (channels[13] >> 9) | (channels[14] << 2);
    sbusPacket.channels[20] = (channels[14] >> 6) | (channels[15] << 5);
    sbusPacket.channels[21] = (channels[15] >> 3);
    sbusPacket.flags = 0;
    if (channels[16] > 1024) {
        sbusPacket.flags |= 0x1;
    }
    if (channels[17] > 1024) {
        sbusPacket.flags |= 0x2;
    }
    if (failsafeEnabled) {
        sbusPacket.flags |= SBUS_FAILSAFE;
    }
    if (!DMAC_ChannelIsBusy(DMAC_CHANNEL_1)) {
        DMAC_ChannelTransfer(DMAC_CHANNEL_1, (void *) &sbusPacket, (void *) &(SERCOM2_REGS->USART_INT.SERCOM_DATA), sizeof (sbusPacket));
    }
}