#ifndef MLLOGGINGAPP_MODEL_H
#define MLLOGGINGAPP_MODEL_H

#include <string.h>
#include <cmath>
#include <array>

class SNRMLModel {
public:
    static SNRMLModel& instance();
    virtual void add_vectors(double *v1, double *v2, int size, double *result);
    virtual void mul_vector_number(double *v1, double num, int size, double *result);
    virtual void score(double *input, double *output); // {log_p_signal, log_p_noise, snr_db}
    virtual std::array<double, 3> convert_to_input(double pSignal, double pNoise);
private:
    SNRMLModel() = default;
    SNRMLModel(const SNRMLModel&) = delete;
    SNRMLModel& operator=(const SNRMLModel&) = delete;
};

#endif // MLLOGGINGAPP_MODEL_H
