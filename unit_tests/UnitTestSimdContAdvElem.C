#include <gtest/gtest.h>

#include <stk_util/environment/WallTime.hpp>
#include <stk_simd/Simd.hpp>
#include <stk_mesh/base/GetEntities.hpp>

#include <UnitTestHelperObjects.h>

#include "UnitTestUtils.h"

#include <SolutionOptions.h>
#include <TimeIntegrator.h>
#include <ContinuityAdvElemKernel.h>
#include <ElemDataRequests.h>
#include <ScratchViews.h>

TEST_F(Hex8MeshWithNSOFields, continuityAdvElem)
{
  fill_mesh_and_initialize_test_fields("generated:10x10x10");

  sierra::nalu::SolutionOptions solnOpts;
  solnOpts.meshMotion_ = false;
  solnOpts.meshDeformation_ = false;
  solnOpts.externalMeshDeformation_ = false;
  solnOpts.cvfemShiftMdot_ = false;
  solnOpts.shiftedGradOpMap_["pressure"] = false;
  solnOpts.cvfemReducedSensPoisson_ = false;
  solnOpts.mdotInterpRhoUTogether_ = true;

  unit_test_utils::HelperObjects helperObjs(bulk, stk::topology::HEX_8, 1, partVec[0]);

  sierra::nalu::TimeIntegrator timeIntegrator;
  timeIntegrator.gamma1_ = 1.0;
  timeIntegrator.timeStepN_ = 1.0;
  timeIntegrator.timeStepNm1_ = 1.0;
  helperObjs.realm.timeIntegrator_ = &timeIntegrator;

  std::unique_ptr<sierra::nalu::Kernel> advKernel(
    new sierra::nalu::ContinuityAdvElemKernel<sierra::nalu::AlgTraitsHex8>(
      bulk, solnOpts, helperObjs.assembleElemSolverAlg->dataNeededBySuppAlgs_));

  helperObjs.assembleElemSolverAlg->activeKernels_.push_back(advKernel.get());

  double startTime = stk::wall_time();

  helperObjs.assembleElemSolverAlg->execute();

  double elapsedTimeSimd = stk::wall_time() - startTime;

  stk::mesh::Selector all = meta.universal_part();
  const stk::mesh::BucketVector& elemBuckets = bulk.get_buckets(stk::topology::ELEM_RANK, all);
  const int numElems = stk::mesh::count_selected_entities(all, elemBuckets);

  std::cerr<<"numElems: "<<numElems<<", elapsedTime Hex8MeshWithNSOFields.continuityAdvElem: "<<elapsedTimeSimd<<std::endl;
}

TEST_F(Hex8MeshWithNSOFields, continuityAdvElem_new_ME)
{
  fill_mesh_and_initialize_test_fields("generated:10x10x10");

  sierra::nalu::SolutionOptions solnOpts;
  solnOpts.meshMotion_ = false;
  solnOpts.meshDeformation_ = false;
  solnOpts.externalMeshDeformation_ = false;
  solnOpts.cvfemShiftMdot_ = false;
  solnOpts.shiftedGradOpMap_["pressure"] = false;
  solnOpts.cvfemReducedSensPoisson_ = false;
  solnOpts.mdotInterpRhoUTogether_ = true;

  unit_test_utils::HelperObjectsNewME helperObjs(bulk, stk::topology::HEX_8, 1, partVec[0]);

  sierra::nalu::TimeIntegrator timeIntegrator;
  timeIntegrator.gamma1_ = 1.0;
  timeIntegrator.timeStepN_ = 1.0;
  timeIntegrator.timeStepNm1_ = 1.0;
  helperObjs.realm.timeIntegrator_ = &timeIntegrator;

  std::unique_ptr<sierra::nalu::Kernel> advKernel(
    new sierra::nalu::ContinuityAdvElemKernel<sierra::nalu::AlgTraitsHex8>(
      bulk, solnOpts, helperObjs.assembleElemSolverAlg->dataNeededBySuppAlgs_));

  helperObjs.assembleElemSolverAlg->activeKernels_.push_back(advKernel.get());

  double startTime = stk::wall_time();

  helperObjs.assembleElemSolverAlg->execute();

  double elapsedTimeSimd = stk::wall_time() - startTime;

  stk::mesh::Selector all = meta.universal_part();
  const stk::mesh::BucketVector& elemBuckets = bulk.get_buckets(stk::topology::ELEM_RANK, all);
  const int numElems = stk::mesh::count_selected_entities(all, elemBuckets);

  std::cerr<<"numElems: "<<numElems<<", elapsedTime Hex8MeshWithNSOFields.continuityAdvElem: "<<elapsedTimeSimd<<std::endl;
}
