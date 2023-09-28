#include <stdint.h>
#include "chirp.h"
#include <stdlib.h>
#include <math.h>

static void __chirp_phase(float* DataList, uint32_t ListLenth, double StartFreq, double EndFreq, double UsedTime) {
	double AverageTime;
	uint32_t Loop;
	if (DataList == NULL) return;
	if (UsedTime <= 0) return;

	AverageTime = (EndFreq - StartFreq) / UsedTime;

	for (Loop = 0; Loop < ListLenth; Loop++) {
		DataList[Loop] = 2 * PI * (StartFreq * DataList[Loop] + 0.5 * AverageTime * DataList[Loop] * DataList[Loop]);
	}

}

float* chirp_f32_cos(float* data, uint32_t PointCount, double StartFreq, double EndFreq, double Samplerate, double phi) {
	return chirp(data, PointCount, StartFreq, EndFreq, PointCount / Samplerate, phi);
}

float* chirp(float* DataList, uint32_t ListLenth, double StartFreq, double EndFreq, double UsedTime, double phi) {
	uint32_t loop;
	static double leftphi = 0;
	if (DataList == NULL) {
		if (ListLenth == 0) return NULL;
		DataList = (float*)malloc(sizeof(float) * ListLenth);
		if (DataList == NULL) return NULL;
	}

	for (loop = 0; loop < ListLenth; loop++) {
		DataList[loop] = loop * UsedTime / ListLenth;
	}


	__chirp_phase(DataList, ListLenth, StartFreq, EndFreq, UsedTime);

	for (loop = 0; loop < ListLenth; loop++) {
		DataList[loop] = cos(DataList[loop] + phi + leftphi);
	}
	leftphi = (loop - 1) * UsedTime / ListLenth + phi + leftphi;
	while (leftphi > 2 * 3.1415926) {
		leftphi -= 2 * 3.1415926;
	}
	return DataList;
}
