#ifndef FRUSTUM_HPP
#define FRUSTUM_HPP
#include "Plane.hpp"

///
/// \brief Represents view frustum for any given View Projection matrix.
///
class Frustum {
	public:
		///
		/// \brief Identifies each of the planes in this frustum
		///
		enum PlaneID {
			TOP = 0, ///< Top plane
			BOTTOM, ///< Bottom plane
			LEFT, ///< Left plane
			RIGHT, ///< Right plane
			NEAR, ///< Near plane
			FAR ///< Far plane
		};

		///
		/// \brief Default constructor
		///
		/// This frustum will be invalid untill recalculated
		///
		/// \see calculate
		///
		Frustum();

		///
		/// \brief Destructor
		///
		~Frustum();

		///
		/// \brief Recalculate collision planes for this Frustum
		///
		/// Will modify the six collision planes to match the given VP matrix.
		///
		/// \param VP A valid VP matrix
		///
		/// \see calculate
		///
		void calculate(mat4f VP);

		///
		/// \brief Get any given collision Plane on this Frustum
		///
		Plane getPlane(PlaneID p);

	private:
		friend class Collision;
		Plane planes[6];
};

///
/// \class Frustum Frustum.hpp "geometry/Frustum.hpp"
///	\ingroup Geometry
///
/// This class can be used to test against other geometry objects,
/// in order to perform fustrum culling for example.
///
/// \see Collision
///
#endif // FRUSTUM_HPP
