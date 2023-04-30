
// Project file headers
#include "Script.hpp"
#include "Color.hpp"
#include "PNG.hpp"
// C++ library headers
#include <algorithm>
#include <cstdlib>
#include <cassert>
#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <iterator>
#include <fstream>
using namespace std;

// POSIX headers
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <dirent.h>

namespace prog
{

  const string LOG_FILE_NAME = "test_log.txt";

  class TestDriver
  {
  private:
    string root_path;
    int total_tests = 0;
    int passed_tests = 0;
    int failed_tests = 0;
    FILE *log_stream;

    bool test_script(const string &id)
    {
      string script_file = root_path + "/scripts/" + id + ".txt";
      string out_file = root_path + "/output/" + id + ".png";
      string exp_file = root_path + "/expected/" + id + ".png";
      Script s(script_file);
      s.run();
      return comparePNG(exp_file, out_file, cout);
    }

    void onTestBegin(const string &id)
    {
      total_tests++;
      fprintf(log_stream, ">>>> [%d] %s <<<<\n", total_tests, id.c_str());
      fflush(log_stream);
      cout << '[' << total_tests << "] " << id << ": ";
      cout.flush();
    }
    void onTestCompletion(bool success)
    {
      cout << (success ? "pass" : "fail") << std::endl;
      if (success)
      {
        passed_tests++;
      }
      else
      {
        failed_tests++;
      }
    }

    
    void run_test(const string &script_id)
    {
      int log_fd = ::fileno(log_stream);
      onTestBegin(script_id);
      ::pid_t pid = ::fork();

      if (pid == 0)
      {
        ::dup2(log_fd, 1);
        ::dup2(log_fd, 2);
        bool success = test_script(script_id);
        ::exit(success ? 0 : 1);
      }
      else if (pid > 0)
      {
        // parent process waits
        int child_status = -1;
        ::waitpid(pid, &child_status, 0);

        bool success = WIFEXITED(child_status) &&
                       WEXITSTATUS(child_status) == 0;
        onTestCompletion(success);
      }
      else
      {
        // pid < 0
        perror("Unable to run tests! Process creation failed!");
        exit(1);
      }
    }

  public:
    TestDriver(const string &root_path)
        : root_path(root_path),
          log_stream(fopen((root_path + "/" + LOG_FILE_NAME).c_str(), "w"))
    {

    }
    void color_tests() {
      const Color a(1, 2, 3);
      assert(a.red() == 1);
      assert(a.green() == 2);
      assert(a.blue() == 3);

      const Color b;
      assert(b.red() == 0);
      assert(b.green() == 0);
      assert(b.blue() == 0);
 
      const Color c(b);
      assert(c.red() == b.red());
      assert(c.green() == b.green());
      assert(c.blue() == b.blue());

      Color d(4, 5, 6);
      d.red() = 111;
      d.green() = 255;
      d.blue() = 33;
      assert(d.red() == 111);
      assert(d.green() == 255);
      assert(d.blue() == 33);
      cout << "Color tests passed!" << endl;
    }
  };
}

int main(int argc, char **argv)
{
  --argc;
  ++argv;
  prog::TestDriver driver(argc == 2 ? argv[1] : ".");
  string spec = argc >=1 ? argv[0] : "";
  driver.color_tests();
  return 0;
}
