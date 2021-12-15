#pragma once

// Holds parameters for a laser and for a particle
struct LaserParticleParameters {
	double d;
	double v;
	double w0;
	double sigma{w0 / v};
};