#pragma once

#include <signal.h>
#include <errno.h>
#include <stdexcept>
#include <string>

using std::runtime_error;

class SignalException : public runtime_error
{
  public:
    SignalException(const std::string& _message) : runtime_error(_message)
    {
    }
};

class SignalHandler
{
  protected:
    static bool mbGotExitSignal;

  public:
    SignalHandler(){}

    ~SignalHandler(){}

    /**
     * Returns the bool flag indicating whether we received an exit signal
     * @return Flag indicating shutdown of program
     */
    static bool gotExitSignal()
    {
      return mbGotExitSignal;
    }

    /**
     * Set the bool flag indicating whether we received an exit signal
     */
    static void setExitSignal(bool _bExitSignal)
    {
      mbGotExitSignal = _bExitSignal;
    }

    /**
     * Set exit signal to true
     * @parma[in] _ignored Not used but required by function prototype
     * to match required handler.
     */
    static void exitSignalHandler(int _ignored)
    {
      mbGotExitSignal = true;
      throw SignalException("segmentation violation is happen.");
    }

    /**
     * Set up the signal handlers for CTRL-C
     */
    void setupSignalHandlers()
    {
      if (signal((int) SIGSEGV, SignalHandler::exitSignalHandler) == SIG_ERR)
      {
        throw SignalException("!!!!! Error setting up signal handlers !!!!!");
      }
    }
};

