// Copyright 2023 Bladesense, Servaas Clerckx
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
// 1. Redistributions of source code must retain the above copyright notice,
//    this list of conditions and the following disclaimer.
//
// 2. Redistributions in binary form must reproduce the above copyright notice,
//    this list of conditions and the following disclaimer in the documentation
//    and/or other materials provided with the distribution.
//
// 3. Neither the name of the copyright holder nor the names of its contributors
//    may be used to endorse or promote products derived from this software
//    without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
// LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.


#include "badslam/input_spectacularai.h"

using namespace std;
using std::cout;
using std::endl;
using json = nlohmann::json;

namespace fs = boost::filesystem;
namespace bp = boost::process;

namespace vis {

    SpectInputThread::~SpectInputThread() {
        exit_ = true;
        if (thread_) {
            LOG(INFO) << "Closing spectacularai input thread";
            thread_->join();
        }
    }

    void SpectInputThread::init_memory() {

    }

    void SpectInputThread::Start() {
        

        // Start thread
        exit_ = false;
        thread_.reset(new thread(std::bind(&SpectInputThread::ThreadMain, this)));
    }

    void SpectInputThread::GetNextFrame() {
        
    }

    void SpectInputThread::ThreadMain() {
        bp::ipstream out; // the output stream

        // Child process
        bp::child c("D:\\Bladesense\\SpectacularAI SDK\\Windows\\bin\\vio_jsonl.exe", bp::std_out > out);

        // Read the output
        std::string line;
        while (out && std::getline(out, line) && !line.empty()) {
            // Parse the line as JSON
            json j = json::parse(line);

            // Access elements of the JSON object
            double x_acc = j["acceleration"]["x"];
            double y_acc = j["acceleration"]["y"];
            double z_acc = j["acceleration"]["z"];

            std::cout << "Acceleration: (" << x_acc << ", " << y_acc << ", " << z_acc << ")\n";
        }

        c.wait();  // Wait for the process to finish
    }

}
