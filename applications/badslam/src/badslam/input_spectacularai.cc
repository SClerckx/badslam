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

const std::string PIPE_NAME = "\\\\.\\pipe\\MyNamedPipe";

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
        void run_command()
        {
            std::string command = "\"D:\\Bladesense\\SpectacularAI SDK\\Windows\\bin\\vio_jsonl.exe\" > " + PIPE_NAME;
            std::system(command.c_str());
        }
        HANDLE hPipe = CreateNamedPipe(
            PIPE_NAME.c_str(),
            PIPE_ACCESS_INBOUND,
            PIPE_TYPE_BYTE | PIPE_READMODE_BYTE | PIPE_WAIT,
            1,
            1024 * 16,
            1024 * 16,
            NMPWAIT_USE_DEFAULT_WAIT,
            NULL);

        if (hPipe == INVALID_HANDLE_VALUE)
        {
            std::cerr << "Error creating named pipe" << std::endl;
        }

        // Run the command asynchronously
        std::future<void> command_future = std::async(std::launch::async, run_command);

        // Connect to the named pipe
        BOOL connected = ConnectNamedPipe(hPipe, NULL) ? TRUE : (GetLastError() == ERROR_PIPE_CONNECTED);

        if (!connected)
        {
            CloseHandle(hPipe);
            std::cerr << "Error connecting to named pipe" << std::endl;
        }

        // Read the output in real-time
        // Read the output in real-time
        DWORD bytesRead;
        char buffer[128 * 8];
        std::string accumulated_data;
        while (ReadFile(hPipe, buffer, sizeof(buffer) - 1, &bytesRead, NULL) != FALSE)
        {
            buffer[bytesRead] = '\0';
            accumulated_data += buffer;

            // Check for the start and end of a JSON object
            size_t start_pos = accumulated_data.find_first_of("{");
            size_t end_pos = accumulated_data.find("}}", start_pos) + 3;

            while (start_pos != std::string::npos && end_pos != std::string::npos)
            {
                // Extract the complete JSON object string
                std::string json_str = accumulated_data.substr(start_pos, end_pos - start_pos + 1);

                //std::cout << "pure string: " << json_str << "end pure string";

                try
                {
                    // Parse the JSON object
                    json j = json::parse(json_str);

                    // Access elements of the JSON object
                    double x_acc = j["acceleration"]["x"];
                    double y_acc = j["acceleration"]["y"];
                    double z_acc = j["acceleration"]["z"];

                    std::cout << "Acceleration: (" << x_acc << ", " << y_acc << ", " << z_acc << ")\n";
                }
                catch (const json::exception& e)
                {
                    std::cerr << "Error parsing JSON: " << e.what() << std::endl;
                }

                // Remove the parsed JSON object from the accumulated data
                accumulated_data.erase(0, end_pos + 1);

                // Check for the next JSON object
                start_pos = accumulated_data.find_first_of("{");
                end_pos = accumulated_data.find_first_of("}", start_pos);
            }
        }

        // Disconnect and close the named pipe
        DisconnectNamedPipe(hPipe);
        CloseHandle(hPipe);
    }

    
}
