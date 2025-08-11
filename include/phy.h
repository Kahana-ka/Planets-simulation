
#ifndef PHY
#define PHY

#include <cmath>
#include <vector>
#include <string>

namespace phy {

	class Vector3 {
	public:
		double x{0}, y{0}, z{0};
		Vector3(const double x,const double y,const double z) : x(x), y(y), z(z) {}
		Vector3() = default;
		Vector3 operator+(const Vector3& rv3) const;
		Vector3 operator-(const Vector3& rv3) const;
		Vector3 operator/(double rh_value) const;
		Vector3 operator*(double rh_value) const;
		Vector3 operator+=(const Vector3& rv3) const;

		double module() const;
		double dot(const Vector3& v3) const;
		Vector3 cross(const Vector3& v3) const;
		double distance(const Vector3& v3) const;

	};

	struct Planet {
		std::vector<Vector3> position;
		std::vector<Vector3> speed;
		std::vector<Vector3> acceleration;
		double mass {0};
		double radius {0};
		std::string id;
	};

	/** Set a planet IC
	*	@param planet, will modified fields
	*	@param position Vector4 with positions
	*   @param speed Vector3 with speeds
	*  	@param mass planet mass
	*   @param radius radius of the planet
	*	@param acceleration Vector3 with accelerations optional
	*/
	void set_ic(Planet& planet,const Vector3& position,const Vector3& speed, double mass, double radius, const Vector3& acceleration = {0,0,0});

	/** Set a planet IC
*	@param planet, will modified fields
*	@param position Vector4 with positions
*   @param speed Vector3 with speeds
*  	@param mass planet mass
*   @param radius radius of the planet
*   @param id string with planet name
*	@param acceleration Vector3 with accelerations optional
*/
	void set_ic(Planet& planet,const Vector3& position,const Vector3& speed,double mass, double radius, std::string_view id,const Vector3& acceleration = {0,0,0});

	/** x componet of gravity acceleration between 2 planets a = m/(sqrt((x1-x2)^2 + (y1-y2)^2))^3 * (x2-x1)
	 * 	 G is set to 1
	* 	@param position of the first planet
	*	@param position of the second planet
	*	@param mass of the other planet
	*/
	double gravity_calc_x(double  mass_p2,const Vector3& position_p1, const Vector3& position_p2 );

	/** y componet of gravity acceleration between 2 planets a = m/(sqrt((x1-x2)^2 + (y1-y2)^2))^3 * (x2-x1)
	 * 	G is set to 1
	* 	@param position of the first planet
	*	@param position of the second planet
	*	@param mass of the other planet
	*/
	double gravity_calc_y(double  mass_p2, const Vector3& position_p1,const Vector3& position_p2 );

	/** z componet of gravity acceleration between 2 planets a = m/(sqrt((x1-x2)^2 + (y1-y2)^2))^3 * (x2-x1)
	 *  G is set to 1
	* 	@param position of the first planet
	*	@param position of the second planet
	*	@param mass of the other planet
	*/
	double gravity_calc_z(double  mass_p2, const Vector3& position_p1, const Vector3& position_p2 );

	phy::Vector3 gravity_calc (double  mass_p2, const Vector3 &planet_1, const Vector3 &planet_2);
}

#endif // !PHY

