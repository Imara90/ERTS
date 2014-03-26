#define SAFE_JUMP 5
/*---------------------------------------------------
 * Slowly decrease the engine RPM's untill there are 0, then clip to 0
 * By Imara Speek 1506374
 *---------------------------------------------------
 */
void safe_mode(void)
{
	int i;

	telemetry_flag &= 0x01;
		
	for (i = 0; i < 4; i++)
	{
		ae[i] = ae[i] - SAFE_JUMP;
		if (ae[i] < 0)
		{
			ae[i] = 0;
		}
	}
}
