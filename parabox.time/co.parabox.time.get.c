/**
 * @file
 * co.parabox.time.get node implementation.
 *
 * @copyright Copyright © 2012–2014 Kosada Incorporated.
 * This code may be modified and distributed under the terms of the MIT License.
 * For more information, see http://vuo.org/license.
 */

#include "node.h"
#include <stdio.h>

VuoModuleMetadata({
					 "title" : "Get Date and Time",
					 "keywords" : [ "second", "minute", "hour", "clock", "watch", "pink floyd", "date", "month", "year" ],
					 "version" : "1.0.0",
					 "description": "Get the current date and time.\n \n - `Year` – The current year.\n - `Month` – The number of months since January (eg, January is 1, February is 2, ... October is 10).\n - `Day` – The current day of the month.\n - `Hour` – The hours since midnight (Midnight is 0, noon is 12, 1pm is 11).\n - `Minute` – The number minutes from the top of the hour.\n - `Second` – The number of seconds from the top of the minute (0 - 59).\n",
					 "dependencies" : [ "" ],
					 "node" : {
						  "exampleCompositions" : [ "" ]
					 }
				 });

void nodeEvent
(
		VuoOutputData(VuoInteger) year,
		VuoOutputData(VuoInteger) month,
		VuoOutputData(VuoInteger) day, 
		VuoOutputData(VuoInteger) hour,
		VuoOutputData(VuoInteger) minute,
		VuoOutputData(VuoInteger) second
)
{
	// Get the current date and time.

	// // - `Year` – The current year.
	// - `Month` – The number of months since January (eg, January is 1, February is 2, ... October is 10).
	// // - `Day` – The current day of the month.
	// - `Hour` – The hours since midnight (Midnight is 0, noon is 12, 1pm is 11).
	// // - `Minute` – The number minutes from the top of the hour.
	// - `Second` – The number of seconds from the top of the minute (0 - 59).


	 // http://stackoverflow.com/questions/1442116/how-to-get-date-and-time-value-in-c-program
	time_t t = time(NULL);
	struct tm tm = *localtime(&t);

	*year = tm.tm_year + 1900;
	*month = tm.tm_mon + 1;
	*day = tm.tm_mday;
	*hour = tm.tm_hour;// * 60;
	*minute = tm.tm_min;
	*second = tm.tm_sec;
}
