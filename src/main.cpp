#include <cmath>
#include <unistd.h> // sleep()

#include <jack_wrapper/jack_wrapper.h>

typedef jack_default_audio_sample_t sample_t;

jack_port_t *inputPort;
jack_port_t *outputPort;

inline sample_t sigmoidDistortionFunction(sample_t x, sample_t gain,
                                          sample_t max, sample_t dc) {
  return max * gain * x / sqrt(1 + (gain * pow(gain * x, 2))) + dc;
}

inline sample_t asymmetricSigmoidDistortionFunction(sample_t x) {
  // Cutoff for chopping top
  static sample_t cutoff = 0.05;
  static sample_t slope = 0.1;
  static sample_t gain = 20;
  static sample_t max = 0.3;
  static sample_t dc = 0;
  // Calculate constant to add to linear region to make it join up with the
  // sigmoid function
  static sample_t b =
      sigmoidDistortionFunction(x, gain, max, dc) - slope * cutoff;
  if (x > cutoff) {
    return slope * x + b;
  } else {
    return sigmoidDistortionFunction(x, gain, max, dc);
  }
}

inline sample_t asymmetricSigmoidDistortionFunction2(sample_t x) {
  // Cutoff for chopping top
  static sample_t cutoff = 0.05;
  static sample_t gain = 20;
  static sample_t max = 0.3;
  static sample_t dc = 0;
  if (x > cutoff) {
    return sigmoidDistortionFunction(sigmoidDistortionFunction(x, gain, max, dc), gain * 2, max, dc);
  } else {
    return sigmoidDistortionFunction(x, gain, max, dc);
  }
}

inline sample_t distortionFunction(sample_t x) {
  if (x < -0.08905) {
    // Assume x >= -1
    // Therefore, this first interval is actually -1 <= x < -0.08905
    return -(3 / 4) * (1 - pow((1 - (-x - 0.032847)), 12) +
                       (1 / 3) * (-x - 0.032847)) +
           0.01;
  } else if (x < 0.320018) {
    return -6.153 * pow(x, 2) + 3.9375 * x;
  } else {
    // Assume x <= 1
    // Therefore, this last interval is actually 0.320018 <= x <= 1
    return 0.630035;
  }
}

/**
 * Callback to process audio.
 */
int processCallback(jack_nframes_t nframes, void *arg) {
  jack_default_audio_sample_t *in, *out;
  in = (jack_default_audio_sample_t *)jack_port_get_buffer(inputPort, nframes);
  out =
      (jack_default_audio_sample_t *)jack_port_get_buffer(outputPort, nframes);
  for (size_t i = 0; i < nframes; ++i) {
    out[i] = asymmetricSigmoidDistortionFunction2(in[i]);
  }

  return 0;
}

int main() {
  JackClient client("Op-Amp Distortion", processCallback, inputPort,
                    outputPort);

  client.open();
  client.run();

  // Keep running.
  sleep(-1);

  return 1;
}
