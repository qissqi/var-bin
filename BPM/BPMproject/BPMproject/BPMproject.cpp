
#include <iostream>
#include <fstream>
#include <cmath>
#include <vector>
#include <numeric>

#include <fftw3.h>// 需要安装FFTW库，用于FFT变换

using namespace std;

// 读取WAV文件
vector<double> read_wav_file(string filename) {
    vector<double> samples;
    ifstream file(filename, ios::binary);

    if (file) {
        file.seekg(0, file.end);
        int length = file.tellg();
        file.seekg(0, file.beg);

        while (file.tellg() < length) {
            short sample;
            file.read((char*)&sample, sizeof(short));
            samples.push_back((double)sample);
        }

        file.close();
    }

    return samples;
}

// 计算FFT
vector<double> compute_fft(vector<double> samples) {
    fftw_complex* out;
    out = (fftw_complex*)fftw_malloc(sizeof(fftw_complex) * samples.size());

    fftw_plan plan;
    plan = fftw_plan_dft_r2c_1d(samples.size(), samples.data(), out, FFTW_ESTIMATE);
    fftw_execute(plan);
    fftw_destroy_plan(plan);

    vector<double> result;
    for (int i = 0; i < samples.size() / 2; i++) {
        double mag = sqrt(out[i][0] * out[i][0] + out[i][1] * out[i][1]);
        result.push_back(mag);
    }

    fftw_free(out);
    return result;
}

// 检测节拍
//double detect_bpm(vector<double> spectrum) {
//    // TODO: 实现节拍检测算法
//    return 0;
//}
double detect_bpm(vector<double> spectrum) {
    const int num_bands = 300;
    const int samples_per_band = spectrum.size() / num_bands;

    vector<double> band_energies(num_bands);

    for (int i = 0; i < num_bands; i++) {
        double energy = 0;
        for (int j = i * samples_per_band; j < (i + 1) * samples_per_band; j++) {
            energy += spectrum[j];
        }
        band_energies[i] = energy / samples_per_band;
    }

    const double threshold_ratio = 1.5;
    vector<int> beats;
    for (int i = 1; i < num_bands; i++) {
        if (band_energies[i] > threshold_ratio * band_energies[i - 1]) {
            beats.push_back(i * samples_per_band);
        }
    }

    vector<double> time_gaps;
    for (int i = 1; i < beats.size(); i++) {
        double gap = (double)(beats[i] - beats[i - 1]) / samples_per_band;
        time_gaps.push_back(gap);
    }

    double average_gap = accumulate(time_gaps.begin(), time_gaps.end(), 0.0) / time_gaps.size();
    double bpm = 60.0 / average_gap;

    return bpm;
}


int main(int argc, char* argv[]) {
    string filename;
    if (argc < 2) {
        cout << "Usage: " << argv[0] << " <filename>" << endl;
        cin >> filename;
        
    }
    else
    {
        filename = argv[1];
    }
    vector<double> samples = read_wav_file(filename);
    vector<double> spectrum = compute_fft(samples);
    double bpm = detect_bpm(spectrum);

    cout << "BPM: " << bpm << endl;

    return 0;
}
