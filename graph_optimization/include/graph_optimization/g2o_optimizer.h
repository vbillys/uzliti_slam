// Copyright (c) 2014, Institute of Computer Engineering (ITI), Universität zu Lübeck
// Jan Frost
// All rights reserved.
//
// This file is part of the uzliti_slam ROS package.
//
// uzliti_slam is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// uzliti_slam is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with uzliti_slam.  If not, see <http://www.gnu.org/licenses/>.

#ifndef G2O_OPTIMIZER
#define G2O_OPTIMIZER

#include <graph_optimization/graph_optimizer.h>

#include <boost/bimap.hpp>
#include <g2o/core/block_solver.h>
#include <g2o/core/robust_kernel_impl.h>
#include <g2o/solvers/csparse/linear_solver_csparse.h>
#include <g2o/solvers/cholmod/linear_solver_cholmod.h>
#include <g2o/solvers/pcg/linear_solver_pcg.h>
#include <transformation_estimation/transformation_filter.h>

typedef g2o::BlockSolver<g2o::BlockSolverTraits<6, 3> >  SlamBlockSolver;
typedef g2o::LinearSolverCSparse<SlamBlockSolver::PoseMatrixType> SlamLinearSolver;
typedef boost::bimaps::bimap<std::string,g2o::HyperGraph::Edge*> g2o_edge_bimap;
typedef boost::bimaps::bimap<std::string,g2o::HyperGraph::Vertex*> g2o_vertex_bimap;

class G2oOptimizer : public GraphOptimizer
{
public:
    G2oOptimizer();
    ~G2oOptimizer();

protected:
    void addGraphImpl(SlamGraph &graph);
    void storeImpl(SlamGraph &graph);
    void optimizeImpl();

    void clear();
    void addVertex(const SlamNode &node);
    void addOdometryEdge(std::string id, SlamGraph &graph);
    void addFeatureEdge(const SlamEdge &edge);
    void setFixedNodes();

    g2o::SparseOptimizer optimizer_;
    SlamLinearSolver *linear_solver_;
    SlamBlockSolver *block_solver_;
    g2o::OptimizationAlgorithm *solver_;

    g2o_vertex_bimap node_id_to_g2o_map_;
    g2o_edge_bimap edge_id_to_g2o_map_;

    bool has_gps_measurement_;
    int optimizer_count_;

    std::map<std::string, Eigen::Isometry3d, std::less<std::string>, Eigen::aligned_allocator<std::pair<const std::string, Eigen::Isometry3d> > > sensor_transforms_;
    boost::shared_ptr<TransformationFilter> edge_filter_;
};

#endif
