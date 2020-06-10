void Send_Frame(struct ETFrame *pFrame){
	Xuint32 index = 0;
	Xuint32 count;
	Xuint8 sdata[nSendByte] = { 0 };

	memcpy(&sdata, pFrame, nSendByte);
	while (index < nSendByte) {
		count = XIOModule_Send(&iomodule, &sdata[index], nSendByte - index);
		index += count;
	}
}





#ifdef bUseFloat
	#define nHT		4	// size of head/tail
	#define	nBytes	4	// number of bytes for each measurement.
	#define fhData 0x41424344
	#define feData 0x5758595A
	struct ETFrame{
		Xuint32 head;
		Xfloat32 data1[nMeas];
	#ifdef bSingleADC
		Xfloat32 data2[nMeas];
	#endif
		Xuint32 tail;
	} ET, *ptET;
#endif

#ifdef bUseInt16
	#define nHT		2
	#define	nBytes	2
	#define fhData 0x4142
	#define feData 0x5758
	struct ETFrame{
		Xuint16 head;
		Xuint16 data1[nMeas];
	#ifdef bSingleADC
		Xuint16 data2[nMeas];
	#endif
		Xuint16 tail;
	} ET, *ptET;
#endif

#ifdef bUseInt8
	#define nHT		2
	#define	nBytes	1
	#define fhData 0x4142
	#define feData 0x5758
	struct ETFrame{
		Xuint16 head;
		Xuint8 data1[nMeas];
	#ifdef bSingleADC
		Xuint8 data2[nMeas];
	#endif
		Xuint16 tail;
	} ET, *ptET;
#endif



// Sensor defines
#define nElectrode 	8		//The number of electrode
#define nPlane		1
#define nBoard		2		//The number of front-end board
#define nMeas 		28		//The number of measurement in a frame
#define nSendByte	(nMeas * nPlane ) * nBytes + 2 * nHT

#define bSingleADC
#define bADC1 		0
#define bADC2 		1

//#define bUseInt8
//#define bUseInt16
#define bUseFloat

