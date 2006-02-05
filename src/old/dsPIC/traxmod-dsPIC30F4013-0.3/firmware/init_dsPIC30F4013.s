
; Initialization Code for dsPIC30F4013, Family: controller control, Package: 40-Pin PDIP 40pins

.include "p30F4013.inc"



; Filter Controls used to Generate Code:

; POR Match Filter ON

; Provisioned Feature Filter ON

; Masks are Ignored and uses UnMasked Register Writes

.GLOBAL _VisualInitialization

; Feature=fuses - fuses (DCR) configuration

; B15:14=FSCKM1:0 B10:8=FOS2:0 B4:0=FPR4:0

;	config __FOSC, 0xC703

; B15=FWDTEN B5:4=FWPSA1:0 B3:0=FWPSB3:0

;	config __FWDT, 0x003F

; B15=MCLREN B7=BOREN B5:4=BORV1:0 B3:0=FPWRT3:0

;	config __FBORPOR, 0x8733

.text

_VisualInitialization:

; Feature=Interrupts - Disable Interrupts during configuration

; clear int flags:

; B15=CN B14=BCL B13=I2C B12=NVM B11=AD B10=U1TX B9=U1RX B8=SPI1

; B7=T3 B6=T2 B5=OC2 B4=IC2 B3=T1 B2=OC1 B1=IC1 B0=INT0

; IFS0 is equal to POR and therefore not initialized.

; B15:12=IC6:3 B11=C1 B10=SPI2 B9=U2TX B8=U2RX

; B7=INT2 B6=T5 B5=T4 B4=OC4 B3=OC3 B2=IC8 B1=IC7 B0=INT1

; IFS1 is equal to POR and therefore not initialized.

; B12=FLTB B11=FLTA B10=LVD B9=DCI B8=QEI

; B7=PWM B6=C2 B5=INT4 B4=INT3 B3:0=OC8:5

; IFS2 is equal to POR and therefore not initialized.

; IEC0 is equal to POR and therefore not initialized.

; IEC1 is equal to POR and therefore not initialized.

; IEC2 is equal to POR and therefore not initialized.

; Feature=Reset - Reset configuration

; B15=TRAPR B14=IOPWR B13=BGST B12=LVDEN B11:8=LVDL3:0

; B7=EXTR B6=SWR B5=SWDTEN B4=WDTO B3=SLEEP B2=IDLE B1=BOR B0=POR

	MOV #0x0500, W0
	MOV W0, RCON

; Feature=DSP - DSP core configuration

; Feature code removed since notprovisioned.

; Feature=NVM - NVM configuration - not implemented

; Feature=Oscillator - Oscillator configuration

; method to override OSCCON write protect

; B13:12=COSC1:0 B9:8=NOSC1:0

	MOV.B #0x03, W0

	MOV.B #0x78, W1

	MOV.B #0x9A, W2

	MOV.W #OSCCON, W3

	MOV.B W1, [W3+1]

	MOV.B W2, [W3+1]

	MOV.B W0, [W3+1]

; B7:6=POST1:0 B5=LOCK B3=CF B1=LPOSCEN B0=OSWEN

	CLR.B W0

	MOV.B #0x46, W1

	MOV.B #0x57, W2

	MOV.B W1, [W3+0]

	MOV.B W2, [W3+0]

	MOV.B W0, [W3+0]

; Feature=A2D - A2D configuration

                    ; force all A2D ports to digital IO at first
	MOV #0xFFFF, W0
	MOV W0, ADPCFG

; Feature=IOPortA - IO Ports configuration

; Feature code removed since does not exist on this device.

; Feature=IOPortB - IO Ports configuration

; B15:0=B15:0

;	CLR PORTB                              ; enable

;	MOV #0xE0C0, W0                        ; direction in=1
;	MOV W0, TRISB

; Feature=IOPortC - IO Ports configuration

; B15:0=C15:0

	CLR PORTC                              ; enable

	MOV #0x1FFF, W0                        ; direction in=1
	MOV W0, TRISC

; Feature=IOPortD - IO Ports configuration

; B15:0=D15:0

	CLR PORTD                              ; enable

	MOV #0xFCFC, W0                        ; direction in=1
	MOV W0, TRISD

; Feature=IOPortE - IO Ports configuration

; Feature code removed since does not exist on this device.

; Feature=IOPortF - IO Ports configuration

; B15:0=F15:0

;	CLR PORTF                              ; enable

;	MOV #0xFFCC, W0                        ; direction in=1
;	MOV W0, TRISF

; Feature=IOPortG - IO Ports configuration

; Feature code removed since does not exist on this device.

; Feature=CN1 - Input Change Notification configuration

; Feature code removed since notprovisioned.

; Feature=Timer1 - Timers configuration

; T1CON is equal to POR and therefore not initialized.

; Feature=Timer2 - Timers configuration

; T2CON is equal to POR and therefore not initialized.

; Feature=Timer3 - Timers configuration

; Feature code removed since notprovisioned.

; Feature=Timer4 - Timers configuration

; Feature code removed since notprovisioned.

; Feature=Timer5 - Timers configuration

; Feature code removed since notprovisioned.

; Feature=Timer1 - Timers configuration

	CLR TMR1                               ; timer register

	MOV #0x024D, W0                        ; period register
	MOV W0, PR1

; Feature=Timer3 - Timers configuration

; Feature code removed since notprovisioned.

; Feature=Timer2 - Timers configuration

	CLR TMR2                               ; timer register

	MOV #0x024D, W0                        ; period register
	MOV W0, PR2

; Feature=Timer5 - Timers configuration

; Feature code removed since notprovisioned.

; Feature=Timer4 - Timers configuration

; Feature code removed since notprovisioned.

; Feature=IC1 - Input Capture configuration

; Feature code removed since notprovisioned.

; Feature=IC2 - Input Capture configuration

; Feature code removed since notprovisioned.

; Feature=IC3 - Input Capture configuration

; Feature code removed since notprovisioned.

; Feature=IC4 - Input Capture configuration

; Feature code removed since notprovisioned.

; Feature=IC5 - Input Capture configuration

; Feature code removed since notprovisioned.

; Feature=IC6 - Input Capture configuration

; Feature code removed since notprovisioned.

; Feature=IC7 - Input Capture configuration

; Feature code removed since notprovisioned.

; Feature=IC8 - Input Capture configuration

; Feature code removed since notprovisioned.

; Feature=OC1 - Turn off OC1 thru OC8

; associated timers need to be turned off first

; OC1CON is equal to POR and therefore not initialized.

; Feature=OC2 - Turn off OC1 thru OC8

; associated timers need to be turned off first

; OC2CON is equal to POR and therefore not initialized.

; Feature=OC3 - Turn off OC1 thru OC8

; associated timers need to be turned off first

; OC3CON is equal to POR and therefore not initialized.

; Feature=OC4 - Turn off OC1 thru OC8

; associated timers need to be turned off first

; OC4CON is equal to POR and therefore not initialized.

; Feature=OC5 - Turn off OC1 thru OC8

; associated timers need to be turned off first

; OC5CON is equal to POR and therefore not initialized.

; Feature=OC6 - Turn off OC1 thru OC8

; associated timers need to be turned off first

; OC6CON is equal to POR and therefore not initialized.

; Feature=OC7 - Turn off OC1 thru OC8

; associated timers need to be turned off first

; OC7CON is equal to POR and therefore not initialized.

; Feature=OC8 - Turn off OC1 thru OC8

; associated timers need to be turned off first

; OC8CON is equal to POR and therefore not initialized.

; Feature=OC1 - Output Compare configuration

; OCnRS:  output compare n secondary register

; OCnR:   output compare 1 main register

; OCnCON: B13=OCSIDL B4=OCFLT B3=OCTSEL B2:0=OCM2:0

; OC1RS is equal to POR and therefore not initialized.

; OC1R is equal to POR and therefore not initialized.

; OC1CON is equal to POR and therefore not initialized.

; Feature=OC2 - Output Compare configuration

; OCnRS:  output compare n secondary register

; OCnR:   output compare 1 main register

; OCnCON: B13=OCSIDL B4=OCFLT B3=OCTSEL B2:0=OCM2:0

; OC2RS is equal to POR and therefore not initialized.

; OC2R is equal to POR and therefore not initialized.

; OC2CON is equal to POR and therefore not initialized.

; Feature=OC3 - Output Compare configuration

; OCnRS:  output compare n secondary register

; OCnR:   output compare 1 main register

; OCnCON: B13=OCSIDL B4=OCFLT B3=OCTSEL B2:0=OCM2:0

; OC3RS is equal to POR and therefore not initialized.

; OC3R is equal to POR and therefore not initialized.

	MOV #0x0006, W0
	MOV W0, OC3CON

; Feature=OC4 - Output Compare configuration

; OCnRS:  output compare n secondary register

; OCnR:   output compare 1 main register

; OCnCON: B13=OCSIDL B4=OCFLT B3=OCTSEL B2:0=OCM2:0

; OC4RS is equal to POR and therefore not initialized.

; OC4R is equal to POR and therefore not initialized.

	MOV #0x0006, W0
	MOV W0, OC4CON

; Feature=OC5 - Output Compare configuration

; OCnRS:  output compare n secondary register

; OCnR:   output compare 1 main register

; OCnCON: B13=OCSIDL B4=OCFLT B3=OCTSEL B2:0=OCM2:0

; OC5RS is equal to POR and therefore not initialized.

; OC5R is equal to POR and therefore not initialized.

; OC5CON is equal to POR and therefore not initialized.

; Feature=OC6 - Output Compare configuration

; OCnRS:  output compare n secondary register

; OCnR:   output compare 1 main register

; OCnCON: B13=OCSIDL B4=OCFLT B3=OCTSEL B2:0=OCM2:0

; OC6RS is equal to POR and therefore not initialized.

; OC6R is equal to POR and therefore not initialized.

; OC6CON is equal to POR and therefore not initialized.

; Feature=OC7 - Output Compare configuration

; OCnRS:  output compare n secondary register

; OCnR:   output compare 1 main register

; OCnCON: B13=OCSIDL B4=OCFLT B3=OCTSEL B2:0=OCM2:0

; OC7RS is equal to POR and therefore not initialized.

; OC7R is equal to POR and therefore not initialized.

; OC7CON is equal to POR and therefore not initialized.

; Feature=OC8 - Output Compare configuration

; OCnRS:  output compare n secondary register

; OCnR:   output compare 1 main register

; OCnCON: B13=OCSIDL B4=OCFLT B3=OCTSEL B2:0=OCM2:0

; OC8RS is equal to POR and therefore not initialized.

; OC8R is equal to POR and therefore not initialized.

; OC8CON is equal to POR and therefore not initialized.

; Feature=SPI1 - SPI configuration

; SPInBUF:    SPI n buffer

; SPInSTAT:   B15=SPIEN B13=SPISIDL B6=SPITBF B5=SPIROV B0=SPIRBF

; SPInCON(H): B14=FRMEN B13=SPIFSD B11=DISSDO B10=MODE16 B9=SMP B8=CKE

; SPInCON(L): B7=SSEN B6=CKP B5=MSTEN B4:2=SPRE2:0 B1:0=PPRE1:0

/*
	MOV SPI1BUF, W0

	MOV #0x8000, W0
	MOV W0, SPI1STAT

	MOV #0x0060, W0
	MOV W0, SPI1CON
*/

; Feature=SPI2 - SPI configuration

; Feature code removed since does not exist on this device.

; Feature=I2C - I2C configuration

; Feature code removed since notprovisioned.

; Feature=UART1 - UART 1 configuration

; Feature code removed since notprovisioned.

; Feature=UART2 - UART 2 configuration

; Feature code removed since notprovisioned.

; Feature=CAN1 - CAN bus configuration

; Feature code removed since notprovisioned.

; Feature=CAN2 - CAN bus configuration

; Feature code removed since does not exist on this device.

; Feature=PWM - Motor PWM configuration

; Feature code removed since does not exist on this device.

; Feature=QEI - QEI configuration

; Feature code removed since does not exist on this device.

; Feature=DCI - DCI configuration

; Feature code removed since notprovisioned.

; Feature=A2D - A2D configuration

; ADCON1 is equal to POR and therefore not initialized.

; B15:0=CSSL15:0

; ADCSSL is equal to POR and therefore not initialized.

; B15:14=CH123NB1:0 B13=CH123SB B12=CH0NB B11:8=CH0SB3:0

; B7:6=CH123NA1:0 B5=CH123SA B4=CH0NA B3:0=CH0SA3:0

; ADCHS is equal to POR and therefore not initialized.

; B15:0=PCFG15:0

	MOV #0x1FFF, W0
	MOV W0, ADPCFG

; B12:8=SAMC4:0 B7=ADRC B5:0=ADCS5:0

; ADCON3 is equal to POR and therefore not initialized.

; B15:13=VCFG2:0 B12=OFFCAL B10=CSCNA B9:8=CHPS1:0

; B7=BUFS B5:2=SMPI B1=BUFM B0=ALTS

; ADCON2 is equal to POR and therefore not initialized.

; B15=ADON B13=ADSIDL B12=ADSTBY B9:8=FORM

; B7:5=SSRC B3=SIMSAM B2=ASAM B1=SAMP B0=CONV

; ADCON1 is equal to POR and therefore not initialized.

; Feature=required - Interrupt flags cleared and interrupt configuration

; interrupt priorities IP

; B14:12=T1 B10:8=OC1 B6:4=IC1 B2:0=INTO

; IPC0 is equal to POR and therefore not initialized.

; B14:12=T3 B10:8=T2 B6:4=OC2 B2:0=IC2

; IPC1 is equal to POR and therefore not initialized.

; B14:12=AD B10:8=U1TX B6:4=U1RX B2:0=SPI1

; IPC2 is equal to POR and therefore not initialized.

; B14:12=CN B10:8= BCLB6:4=I2C B2:0=NVM

; IPC3 is equal to POR and therefore not initialized.

; B14:12=OC3 B10:8=IC8 B6:4=IC7 B2:0=INT1

; IPC4 is equal to POR and therefore not initialized.

; B14:12=INT2 B10:8=T5 B6:4=T4 B2:0=OC4

; IPC5 is equal to POR and therefore not initialized.

; B14:12=C1 B10:8=SPI2 B6:4=U2TX B2:0=U2RX

; IPC6 is equal to POR and therefore not initialized.

; B14:12=IC6 B10:8=IC5 B6:4=IC4 B2:0=IC3

; IPC7 is equal to POR and therefore not initialized.

; B14:12=OC8 B10:8=OC7 B6:4=OC6 B2:0=OC5

; IPC8 is equal to POR and therefore not initialized.

; B14:12=PWM B10:8=C2 B6:4=INT4 B2:0=INT3

; IPC9 is equal to POR and therefore not initialized.

; B14:12=FLTA B10:8=LVD B6:4=DCI B2:0=QEI

; IPC10 is equal to POR and therefore not initialized.

; external interrupt enables

; B15=NSTDIS B10=OVATE B9=OVBTE B8=COVTE

; B4=MATHERR B3=ADDRERR B2=STKERR B1=OSCFAIL

; INTCON1 is equal to POR and therefore not initialized.

; B15=ALTIVT B4:0=INTnEP4:0

; INTCON2 is equal to POR and therefore not initialized.

; Feature=Timer1 - Start timers

; Timers1: B15=TON B13=TSIDL B5=TGATE B5:4=TCKPS1:0 B2=TSYNC B1=TCS

	MOV #0x0004, W0
	MOV W0, T1CON

; Feature=Timer3 - Start timers

; Feature code removed since notprovisioned.

; Feature=Timer2 - Start timers

; Timers2,4:   B15=TON B13=TSIDL B5=TGATE B5:4=TCKPS1:0 B3=T32 B1=TCS

	MOV #0x8000, W0
	MOV W0, T2CON

; Feature=Timer5 - Start timers

; Feature code removed since notprovisioned.

; Feature=Timer4 - Start timers

; Feature code removed since notprovisioned.

; Feature=CPU - CPU register configuration

	CLR SR

	CLR SR

	CLR W0

	CLR W1

	CLR W2

; Feature=Interrupts - enable interrupts

; feature interrupt enables IE

; B15=CN B14=BCL B13=I2C B12=NVM B11=AD B10=U1TX B9=U1RX B8=SPI1

; B7=T3 B6=T2 B5=OC2 B4=IC2 B3=T1 B2=OC1 B1=IC1 B0=INT0

; IEC0 is equal to POR and therefore not initialized.

; B15:12=IC6:3 B11=C1 B10=SPI2 B9=U2TX B8=U2RX

; B7=INT2 B6=T5 B5=T4 B4=OC4 B3=OC3 B2=IC8 B1=IC7 B0=INT1

; IEC1 is equal to POR and therefore not initialized.

; B12=FLTB B11=FLTA B10=LVD B9=DCI B8=QEI

; B7=PWM B6=C2 B5=INT4 B4=INT3 B3:0=OC8:5

; IEC2 is equal to POR and therefore not initialized.

	return
