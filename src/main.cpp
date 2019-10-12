#include <unistd.h> // sleep()

#include <jack_wrapper/jack_wrapper.h>

jack_port_t *inputPort;
jack_port_t *outputPort;

/**
 * Callback to process audio.
 */
int processCallback(jack_nframes_t nframes, void *arg) {
  jack_default_audio_sample_t *in, *out;
  in = (jack_default_audio_sample_t *)jack_port_get_buffer(inputPort, nframes);
  out =
      (jack_default_audio_sample_t *)jack_port_get_buffer(outputPort, nframes);
  for (size_t i = 0; i < nframes; ++i) {
    jack_default_audio_sample_t tmp = in[i];
    // Creates nonlinear distortion
    tmp = 3 * tmp * tmp;
    out[i] = tmp;
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
