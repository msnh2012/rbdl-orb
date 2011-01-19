#include <QDebug>
#include <assert.h>

#include "modelstate.h"

#include "mathutils.h"
#include "Model.h"
#include "Dynamics_stdvec.h"

static Model* model = NULL;
static unsigned int body_a_id, body_b_id, body_c_id, body_d_id, ref_body_id;
static Body body_a, body_b, body_c, body_d;
static Joint joint_a, joint_b, joint_c, joint_d;

std::vector<double> Q;
std::vector<double> QDot;
std::vector<double> QDDot;
std::vector<double> Tau;

void model_init () {
	model = new Model();
	model->Init();
	body_a = Body (1., Vector3d (0.5, 0., 0.0), Vector3d (1., 1., 1.));
	joint_a = Joint(
			JointTypeRevolute,
			Vector3d (0., 0., 1.)
			);

	body_a_id = model->AddBody(0, Xtrans(Vector3d(0., 0., 0.)), joint_a, body_a);

	body_b = Body (1., Vector3d (0., 0.5, 0.), Vector3d (1., 1., 1.));
	joint_b = Joint (
			JointTypeRevolute,
			Vector3d (0., 0., 1.)
			);

	body_b_id = model->AddBody(body_a_id, Xtrans(Vector3d(1., 0., 0.)), joint_b, body_b);

	body_c = Body (0., Vector3d (0.5, 0., 0.), Vector3d (1., 1., 1.));
	joint_c = Joint (
			JointTypeRevolute,
			Vector3d (0., 0., 1.)
			);

	body_c_id = model->AddBody(body_b_id, Xtrans(Vector3d(0., 1., 0.)), joint_c, body_c);

	body_d = Body (1., Vector3d (0.5, 0., 0.2), Vector3d (1., 1., 1.));
	joint_d = Joint (
			JointTypeRevolute,
			Vector3d (1., 0., 0.)
			);

	body_d_id = model->AddBody(body_c_id, Xtrans(Vector3d(0., 0., 0.)), joint_d, body_d);

	Q = std::vector<double> (model->mBodies.size() - 1, 0.);
	QDot = std::vector<double> (model->mBodies.size() - 1, 0.);
	QDDot = std::vector<double> (model->mBodies.size() - 1, 0.);
	Tau = std::vector<double> (model->mBodies.size() - 1, 0.);

	// we call model_update once to update the internal variables for the
	// state, etc.
	model_update (0.);
}

void model_update (const double delta_time) {
//	qDebug() << "model = " << model;
	ForwardDynamics (*model, Q, QDot, Tau, QDDot);

	int i;
	for (i = 0; i < Q.size(); i++) {
		Q[i] += delta_time * QDot[i];
		QDot[i] += delta_time * QDDot[i];
	}
}

Model* model_get() {
	assert (model);
	
	return model;
}

void model_destroy () {
	if (model)
		delete model;

	model = NULL;
}