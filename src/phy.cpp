
#include "phy.h"


phy::Vector3 phy::Vector3::operator+(const Vector3& rv3) const {
    return {x+rv3.x,y+rv3.y,z+rv3.z};
}

phy::Vector3 phy::Vector3::operator-(const Vector3& rv3) const {
    return {x-rv3.x,y-rv3.y,z-rv3.z};
}

phy::Vector3 phy::Vector3::operator/(const double rh_value) const {
    return {x/rh_value,y/rh_value,z/rh_value};
}

phy::Vector3 phy::Vector3::operator*(const double rh_value) const {
    return {x*rh_value,y*rh_value,z*rh_value};
}

double phy::Vector3::module() const {
    return std::sqrt(x*x+y*y+z*z);
}

double phy::Vector3::dot(const Vector3& v3) const {
    return x*v3.x + y*v3.y + z*v3.z;
}

phy::Vector3 phy::Vector3::cross(const Vector3& v3) const {
    return {y*v3.z-z*v3.y,z*v3.x-x*v3.z,x*v3.y-y*v3.x};
}

double phy::Vector3::distance(const Vector3& v3) const {
    return sqrt(std::pow(x-v3.x,2)+std::pow(y-v3.y,2)+std::pow(z-v3.z,2));
}

void phy::set_ic(Planet& planet,const Vector3& position,const Vector3& speed, const double mass, const double radius, const Vector3& acceleration) {
    set_ic(planet,position,speed,mass,radius,"",acceleration);
}

void phy::set_ic(Planet& planet,const Vector3& position,const Vector3& speed, const double mass, const double radius,const std::string_view id,const Vector3& acceleration) {
    planet.position.push_back(position);
    planet.speed.push_back(speed);
    planet.mass = mass;
    planet.radius = radius;
    planet.id = id;
    planet.acceleration.push_back(acceleration);
}

double phy::gravity_calc_x(const double  mass_p2,const Vector3& position_p1, const Vector3& position_p2 ) {
    return mass_p2/pow( position_p1.distance(position_p2) ,3) * (position_p2.x - position_p1.x);
}

double phy::gravity_calc_y(const double  mass_p2,const Vector3& position_p1, const Vector3& position_p2 ) {
    return mass_p2/pow( position_p1.distance(position_p2) ,3) * (position_p2.y - position_p1.y);
}

double phy::gravity_calc_z(const double  mass_p2,const Vector3& position_p1, const Vector3& position_p2 ) {
    return mass_p2/pow( position_p1.distance(position_p2) ,3) * (position_p2.z - position_p1.z);
}
