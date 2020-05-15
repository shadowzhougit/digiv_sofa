/******************************************************************************
*       SOFA, Simulation Open-Framework Architecture, development version     *
*                (c) 2006-2019 INRIA, USTL, UJF, CNRS, MGH                    *
*                                                                             *
* This program is free software; you can redistribute it and/or modify it     *
* under the terms of the GNU Lesser General Public License as published by    *
* the Free Software Foundation; either version 2.1 of the License, or (at     *
* your option) any later version.                                             *
*                                                                             *
* This program is distributed in the hope that it will be useful, but WITHOUT *
* ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or       *
* FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License *
* for more details.                                                           *
*                                                                             *
* You should have received a copy of the GNU Lesser General Public License    *
* along with this program. If not, see <http://www.gnu.org/licenses/>.        *
*******************************************************************************
* Authors: The SOFA Team and external contributors (see Authors.txt)          *
*                                                                             *
* Contact information: contact@sofa-framework.org                             *
******************************************************************************/
#ifndef SOFA_COMPONENT_ENGINE_SHAPEMATCHING_H
#define SOFA_COMPONENT_ENGINE_SHAPEMATCHING_H
#include "config.h"



#include <sofa/core/DataEngine.h>
#include <sofa/core/objectmodel/BaseObject.h>
#include <sofa/core/VecId.h>
#include <sofa/core/behavior/MechanicalState.h>
#include <sofa/defaulttype/RigidTypes.h>
#include <SofaBaseTopology/TriangleSetTopologyContainer.h>
#include <SofaBaseTopology/TriangleSetGeometryAlgorithms.h>
#include <sofa/defaulttype/Vec.h>
#include <sofa/helper/SVector.h>


namespace sofa
{

namespace component
{

namespace engine
{

/**
 * This class computes target positions using shape matching deformation [Muller05][Muller11]
 */
template <class DataTypes>
class ShapeMatching : public core::DataEngine
{
public:
    SOFA_CLASS(SOFA_TEMPLATE(ShapeMatching,DataTypes),core::DataEngine);
    typedef typename DataTypes::Coord Coord;
    typedef typename DataTypes::VecCoord VecCoord;
    typedef typename Coord::value_type Real;
    typedef defaulttype::Mat<3,3,Real> Mat3x3;

    typedef core::topology::BaseMeshTopology::PointID ID;
    typedef helper::vector<ID> VI;
    typedef helper::vector<VI> VVI;
	typedef helper::vector<Real> VD;	

public:

    ShapeMatching();

    ~ShapeMatching() override {}

    void init() override;

    void reinit() override;

    void doUpdate() override;

    void draw(const core::visual::VisualParams* vparams) override;

    Data<unsigned int> iterations; ///< Number of iterations.
    Data< Real > affineRatio; ///< Blending between affine and rigid.
    Data< Real > fixedweight; ///< weight of fixed particles.
    Data< VecCoord > fixedPosition0; ///< rest positions of non mechanical particles.
    Data< VecCoord > fixedPosition; ///< current (fixed) positions of non mechanical particles.
    Data< VecCoord > position; ///< input (current mstate position)
    Data< VVI > cluster; ///< input2 (clusters)
    Data< VecCoord > targetPosition;       ///< result

private:
    sofa::core::behavior::MechanicalState<DataTypes>* mstate;
    sofa::core::topology::BaseMeshTopology* topo;

    //rest data
    unsigned int oldRestPositionSize;
    Real oldfixedweight;
    VecCoord Xcm0;
    VecCoord Xcm;
    helper::vector<unsigned int> nbClust;
    VD W;

    helper::vector<Mat3x3> Qxinv; // Qx = sum(X0-Xcm0)(X0-Xcm0)^T
    helper::vector<Mat3x3> T;
};


#if  !defined(SOFA_COMPONENT_ENGINE_SHAPEMATCHING_CPP)
extern template class SOFA_GENERAL_ENGINE_API ShapeMatching<defaulttype::Vec3Types>;
extern template class SOFA_GENERAL_ENGINE_API ShapeMatching<defaulttype::Rigid3Types>;
 
#endif

} // namespace engine

} // namespace component

} // namespace sofa

#endif
