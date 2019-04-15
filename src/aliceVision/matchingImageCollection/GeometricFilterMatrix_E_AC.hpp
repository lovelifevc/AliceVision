// This file is part of the AliceVision project.
// Copyright (c) 2016 AliceVision contributors.
// Copyright (c) 2012 openMVG contributors.
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file,
// You can obtain one at https://mozilla.org/MPL/2.0/.

#pragma once

#include <aliceVision/types.hpp>
#include <aliceVision/matching/IndMatch.hpp>
#include <aliceVision/matchingImageCollection/GeometricFilterMatrix.hpp>
#include <aliceVision/multiview/essential.hpp>
#include <aliceVision/multiview/relativePose/Essential5PSolver.hpp>
#include <aliceVision/multiview/relativePose/FundamentalError.hpp>
#include <aliceVision/robustEstimation/ACRansac.hpp>
#include <aliceVision/robustEstimation/RansacKernel.hpp>
#include <aliceVision/robustEstimation/guidedMatching.hpp>
#include <aliceVision/feature/RegionsPerView.hpp>
#include <aliceVision/sfmData/SfMData.hpp>

#include <limits>

namespace aliceVision {
namespace matchingImageCollection {

/**
 * @brief A contrario essential matrix estimation template functor used for filter pair of putative correspondences
 */
struct GeometricFilterMatrix_E_AC : public GeometricFilterMatrix
{
  GeometricFilterMatrix_E_AC(double dPrecision = std::numeric_limits<double>::infinity(),
                             std::size_t iteration = 1024)
    : GeometricFilterMatrix(dPrecision, std::numeric_limits<double>::infinity(), iteration)
    , m_E(Mat3::Identity())
  {}

  /**
   * @brief Given two sets of image points, it estimates the essential matrix
   *        relating them using a robust method (like A Contrario Ransac).
   */
  template<typename Regions_or_Features_ProviderT>
  EstimationStatus geometricEstimation(const sfmData::SfMData* sfmData,
                                       const Regions_or_Features_ProviderT& regionsPerView,
                                       const Pair& pairIndex,
                                       const matching::MatchesPerDescType& putativeMatchesPerType,
                                       matching::MatchesPerDescType& out_geometricInliersPerType)
  {
    using namespace aliceVision;
    using namespace aliceVision::robustEstimation;

    out_geometricInliersPerType.clear();

    // get back corresponding view index
    const IndexT I = pairIndex.first;
    const IndexT J = pairIndex.second;

    const std::vector<feature::EImageDescriberType> descTypes = regionsPerView.getCommonDescTypes(pairIndex);

    if(descTypes.empty())
      return EstimationStatus(false, false);

    // reject pair with missing Intrinsic information
    const sfmData::View& viewI = sfmData->getView(I);
    const sfmData::View& viewJ = sfmData->getView(J);

    // check that valid cameras can be retrieved for the pair of views
    const camera::IntrinsicBase* camI = sfmData->getIntrinsicPtr(viewI.getIntrinsicId());
    const camera::IntrinsicBase* camJ = sfmData->getIntrinsicPtr(viewJ.getIntrinsicId());

    if(!camI || !camJ)
      return EstimationStatus(false, false);

    if(!isPinhole(camI->getType()) || !isPinhole(camJ->getType()))
      return EstimationStatus(false, false);

    // get corresponding point regions arrays
    Mat xI,xJ;
    fillMatricesWithUndistortFeaturesMatches(pairIndex, putativeMatchesPerType, sfmData, regionsPerView, descTypes, xI, xJ);

    // define the AContrario adapted Essential matrix solver
    typedef RelativePoseKernel_K<
        multiview::relativePose::Essential5PSolver,
        multiview::relativePose::FundamentalEpipolarDistanceError,
        multiview::Mat3Model>
        KernelT;

    const camera::Pinhole* ptrPinholeI = (const camera::Pinhole*)(camI);
    const camera::Pinhole* ptrPinholeJ = (const camera::Pinhole*)(camJ);

    const KernelT kernel(xI, viewI.getWidth(), viewI.getHeight(),
                         xJ, viewJ.getWidth(), viewJ.getHeight(), ptrPinholeI->K(), ptrPinholeJ->K());

    // robustly estimate the Essential matrix with A Contrario ransac
    const double upperBoundPrecision = Square(m_dPrecision);

    std::vector<std::size_t> inliers;
    multiview::Mat3Model model;
    const std::pair<double,double> ACRansacOut = ACRANSAC(kernel, inliers, m_stIteration, &model, upperBoundPrecision);
    m_E = model.getMatrix();

    if (inliers.empty())
      return EstimationStatus(false, false);

    m_dPrecision_robust = ACRansacOut.first;

    // fill geometricInliersPerType with inliers from putativeMatchesPerType
    copyInlierMatches(inliers,
                      putativeMatchesPerType,
                      descTypes,
                      out_geometricInliersPerType);

    // check if resection has strong support
    const bool hasStrongSupport = robustEstimation::hasStrongSupport(out_geometricInliersPerType, kernel.getMinimumNbRequiredSamples());

    return EstimationStatus(true, hasStrongSupport);
  }

  /**
   * @brief Geometry_guided_matching
   * @param sfmData
   * @param regionsPerView
   * @param imageIdsPair
   * @param dDistanceRatio
   * @param matches
   * @return
   */
  bool Geometry_guided_matching(const sfmData::SfMData* sfmData,
                                const feature::RegionsPerView& regionsPerView,
                                const Pair imageIdsPair,
                                const double dDistanceRatio,
                                matching::MatchesPerDescType& matches) override
  {
    if(m_dPrecision_robust != std::numeric_limits<double>::infinity())
    {
      // get back corresponding view index
      const IndexT I = imageIdsPair.first;
      const IndexT J = imageIdsPair.second;

      const sfmData::View& viewI = sfmData->getView(I);
      const sfmData::View& viewJ = sfmData->getView(J);

      // check that valid cameras can be retrieved for the pair of views
      const camera::IntrinsicBase* camI = sfmData->getIntrinsics().count(viewI.getIntrinsicId()) ?
                                          sfmData->getIntrinsics().at(viewI.getIntrinsicId()).get() : nullptr;
      const camera::IntrinsicBase* camJ = sfmData->getIntrinsics().count(viewJ.getIntrinsicId()) ?
                                          sfmData->getIntrinsics().at(viewJ.getIntrinsicId()).get() : nullptr;
      if(!camI || !camJ)
        return false;

      if(!isPinhole(camI->getType()) || !isPinhole(camJ->getType()))
        return false;

      const camera::Pinhole* ptrPinholeI = (const camera::Pinhole*)(camI);
      const camera::Pinhole* ptrPinholeJ = (const camera::Pinhole*)(camJ);

      Mat3 F;
      fundamentalFromEssential(m_E, ptrPinholeI->K(), ptrPinholeJ->K(), &F);

      multiview::Mat3Model model(F);
      // multiview::relativePose::FundamentalSymmetricEpipolarDistanceError
      robustEstimation::guidedMatching<multiview::Mat3Model, multiview::relativePose::FundamentalEpipolarDistanceError>(
            model,
            camI, regionsPerView.getAllRegions(I),
            camJ, regionsPerView.getAllRegions(J),
            Square(m_dPrecision_robust), Square(dDistanceRatio),
            matches);
    }

    return matches.getNbAllMatches() != 0;
  }

  // stored data
  Mat3 m_E;
};

} // namespace matchingImageCollection
} // namespace aliceVision


