/* -------------------------------- Arctic Core ------------------------------
 * Arctic Core - the open source AUTOSAR platform http://arccore.com
 *
 * Copyright (C) 2009  ArcCore AB <contact@arccore.com>
 *
 * This source code is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License version 2 as published by the
 * Free Software Foundation; See <http://www.gnu.org/licenses/old-licenses/gpl-2.0.txt>.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
 * or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
 * for more details.
 * -------------------------------- Arctic Core ------------------------------*/








#include "Com_Sched.h"
#include <string.h>
#include "Trace.h"

#define timerDec(timer) \
	if (timer > 0) { \
		timer = timer - 1; \
	} \


void Com_MainFunctionRx() {
	//DEBUG(DEBUG_MEDIUM, "Com_MainFunctionRx() excecuting\n");
	const ComSignal_type *signal;
	for (int i = 0; !ComConfig->ComSignal[i].ComEcoreEOL; i++) {
		signal = &ComConfig->ComSignal[i];
		ComGetEcoreSignal(signal->ComHandleId);
		ComGetEcoreIPdu(EcoreSignal->ComIPduHandleId);

		// Monitor signal reception deadline
		if (EcoreIPdu->ComEcoreIpduStarted && EcoreSignal->ComTimeoutFactor > 0) {

			// Decrease deadline monitoring timer.
			timerDec(EcoreSignal->ComEcoreDeadlineCounter);

			// Check if a timeout has occurred.
			if (EcoreSignal->ComEcoreDeadlineCounter == 0) {
				if (signal->ComRxDataTimeoutAction == COM_TIMEOUT_DATA_ACTION_REPLACE) {
					// Replace signal data.
					uint32 signalInitData;
					memset(&signalInitData, signal->ComSignalInitValue, sizeof(uint32));

					Com_CopyData(EcoreIPdu->ComIPduDataPtr, &signalInitData, signal->ComBitSize, signal->ComBitPosition, 0);

				}

				// A timeout has occurred.
				if (signal->ComTimeoutNotification != NULL) {
					signal->ComTimeoutNotification();
				}

				// Restart timer
				EcoreSignal->ComEcoreDeadlineCounter = EcoreSignal->ComTimeoutFactor;
			}
		}

		if (EcoreSignal->ComSignalUpdated) {
			ComConfig->ComSignal[i].ComNotification();
			EcoreSignal->ComSignalUpdated = 0;
		}
	}
}


void Com_MainFunctionTx() {
	//DEBUG(DEBUG_MEDIUM, "Com_MainFunctionTx() excecuting\n");
	// Decrease timers.
	const ComIPdu_type *IPdu;
	for (int i = 0; !ComConfig->ComIPdu[i].ComEcoreEOL; i++) {
		IPdu = &ComConfig->ComIPdu[i];
		ComGetEcoreIPdu(i);

		// Is this a IPdu that should be transmitted?
		if (IPdu->ComIPduDirection == SEND && EcoreIPdu->ComEcoreIpduStarted) {
			// Decrease minimum delay timer
			timerDec(EcoreIPdu->ComEcoreTxIPduTimers.ComTxIPduMinimumDelayTimer);

			// If IPDU has periodic or mixed transmission mode.
			if (IPdu->ComTxIPdu.ComTxModeTrue.ComTxModeMode == PERIODIC
				|| IPdu->ComTxIPdu.ComTxModeTrue.ComTxModeMode == MIXED) {

				timerDec(EcoreIPdu->ComEcoreTxIPduTimers.ComTxModeTimePeriodTimer);

				// Is it time for a direct transmission?
				if (IPdu->ComTxIPdu.ComTxModeTrue.ComTxModeMode == MIXED
					&& EcoreIPdu->ComEcoreTxIPduTimers.ComTxIPduNumberOfRepetitionsLeft > 0) {

					timerDec(EcoreIPdu->ComEcoreTxIPduTimers.ComTxModeRepetitionPeriodTimer);

					// Is it time for a transmission?
					if (EcoreIPdu->ComEcoreTxIPduTimers.ComTxModeRepetitionPeriodTimer == 0
						&& EcoreIPdu->ComEcoreTxIPduTimers.ComTxIPduMinimumDelayTimer == 0) {

						Com_TriggerIPduSend(IPdu->ComIPduRxHandleId);

						// Reset periodic timer
						EcoreIPdu->ComEcoreTxIPduTimers.ComTxModeRepetitionPeriodTimer = IPdu->ComTxIPdu.ComTxModeTrue.ComTxModeRepetitionPeriodFactor;

						// Register this nth-transmission.
						EcoreIPdu->ComEcoreTxIPduTimers.ComTxIPduNumberOfRepetitionsLeft--;
					}
				}

				// Is it time for a cyclic transmission?
				if (EcoreIPdu->ComEcoreTxIPduTimers.ComTxModeTimePeriodTimer == 0 && EcoreIPdu->ComEcoreTxIPduTimers.ComTxIPduMinimumDelayTimer == 0) {

					Com_TriggerIPduSend(IPdu->ComIPduRxHandleId); // Send IPDU!

					// Reset periodic timer.
					EcoreIPdu->ComEcoreTxIPduTimers.ComTxModeTimePeriodTimer = IPdu->ComTxIPdu.ComTxModeTrue.ComTxModeTimePeriodFactor;
				}

			// If IPDU has direct transmission mode.
			} else if (IPdu->ComTxIPdu.ComTxModeTrue.ComTxModeMode == DIRECT) {
				// Do we need to transmit anything?
				if (EcoreIPdu->ComEcoreTxIPduTimers.ComTxIPduNumberOfRepetitionsLeft > 0) {
					timerDec(EcoreIPdu->ComEcoreTxIPduTimers.ComTxModeRepetitionPeriodTimer);

					// Is it time for a transmission?
					if (EcoreIPdu->ComEcoreTxIPduTimers.ComTxModeRepetitionPeriodTimer == 0 && EcoreIPdu->ComEcoreTxIPduTimers.ComTxIPduMinimumDelayTimer == 0) {
						Com_TriggerIPduSend(IPdu->ComIPduRxHandleId);

						// Reset periodic timer
						EcoreIPdu->ComEcoreTxIPduTimers.ComTxModeRepetitionPeriodTimer = IPdu->ComTxIPdu.ComTxModeTrue.ComTxModeRepetitionPeriodFactor;

						// Register this nth-transmission.
						EcoreIPdu->ComEcoreTxIPduTimers.ComTxIPduNumberOfRepetitionsLeft--;
					}
				}

			// The IDPU has NONE transmission mode.
			} else {
				// Don't send!
			}
		}
	}

	// Send scheduled packages.
	// Cyclic
	for (int i = 0; !ComConfig->ComIPdu[i].ComEcoreEOL; i++) {
		if (ComConfig->ComIPdu[i].ComIPduDirection == SEND) {

		}
	}
}
