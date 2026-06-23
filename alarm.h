#ifndef ALARM_H
#define ALARM_H

/* Returns 1 if sensor_value exceeds threshold, 0 otherwise.
 * Pure function — no I/O, no globals — so it's easy to extend with
 * hysteresis, debounce, or multi-level alarms without touching anything
 * else in the project. */
int alarm_evaluate(int sensor_value, int threshold);

#endif /* ALARM_H */
