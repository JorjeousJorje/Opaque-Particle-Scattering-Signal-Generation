#pragma once


struct LaserParticleParameters {
	double d;
	double v;
	double w0;
	double sigma{w0 / v};
};