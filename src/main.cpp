#include <cmath>
#include <unistd.h> // sleep()

#include <jack_wrapper/jack_wrapper.h>

jack_port_t *inputPort;
jack_port_t *outputPort;

inline jack_default_audio_sample_t
distortionFunction(jack_default_audio_sample_t x) {
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
    out[i] = distortionFunction(in[i]);
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
