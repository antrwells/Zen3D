#pragma once


	/// <summary>
	/// The maths class contains several helper functions to aid your applications.
	/// </summary>
	class Maths
	{
	public:

		/// <summary>
		/// Returns the PI constant.Which is 3.14159265359
		/// </summary>
		/// <returns></returns>
		static float PI() {

			return (float)3.14159265359;
			
		}

		/// <summary>
		/// Returns a degree as radians.
		/// </summary>
		/// <param name="degree"></param>
		/// <returns></returns>
		static float Deg2Rad(float degree) {

			return (degree * (PI() / 180.0f));

		}

		static float Rad2Deg(float rad) {

			return (float)(rad * (180.0 / 3.141592653589793238463));


		}

		static float CorrectAngle(float ang) {

			if (ang < 0.0f) ang = 360.0f + ang;
			if (ang > 360.0f) ang = ang - 360.0f;
			return ang;

		}

		static double getAbsoluteDiff2Angles(const double x, const double y, const double c)
		{
			// c can be PI (for radians) or 180.0 (for degrees);
			return c - fabs(fmod(fabs(x - y), 2 * c) - c);
		}

		static float ClosetAngle(float ang1, float ang2) {

			ang1 = CorrectAngle(ang1);
			ang2 = CorrectAngle(ang2);

			if (ang1 > ang2)
			{
				float d1 = ang1 - ang2;
				float d2 = ang2 + (360.0f - ang1);
				if (d1 < d2) {
					return -d1;
				}
				else {
					d2;
				}
			}
			else {
				float d1 = ang2 - ang1;
				float d2 = ang1 + (360.0f - ang2);
				if (d1 < d2)
				{
					return d1;
				}
				else {
					return -d2;
				}
			}

		}

	};


