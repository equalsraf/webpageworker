# WEBPAGEWORKER 1

# SYNOPSIS

webpageworker [OPTIONS] URL

# DESCRIPTION

A headless webpage crawler that uses QtWebKit for javascript crawling.
URL is the URL of the page to be fetched, upon getting the page the
worker will execute a user supplied script.

# OPTIONS

**--script** FILE
  A javascript FILE that will be executed upon page load.

**--stdin**
  Alternatively javascript can be read from STDIN.

Upon running the user supplied script the worker will not exit until
the script calls `WebPage.done()`, failing to do so will cause the
worker to run indefinitely. This behaviour can be adjusted using

**--quit-on-load**
  Exit as soon as the user script is finished running.

**--timeout SECONDS**
  Exit after the given number of SECONDS passed.

Debugging of user scripts can be done be enabling a web view or
the webkit inspector.

**--view**
  Displays the webpage in a webview.

**--inspector**
  Opens the webkit inspector. This options prevents the worker from
  exiting (i.e. disables **--timeout**, **--quit-on-load**).

Other options

**--user-agent** UA
  Specify the user agent string to be used for requests

**--help**
  For a list of all options.

# OUTPUT

WebPageWorker is primarily meant to be called by other programs, the output is a
json dict with all the data gathered through the `WebPage` object, e.g.  calls
to `print()` will be stored under `print`

    echo "WebPage.print('hello world!!!')" | ./webpageworker --quit-on-load --stdin "https://google.com"
    {
        "print": [
            "hello world!!!"
        ]
    }

Other attributes are also reserved.

**cookies**
  Holds a list of all the cookies that were set during execution.

# WRITING WORKER SCRIPTS

All worker methods are available under the **WebPage** object

**WebPage.done\(exitcode\)**
  Exits the worker with exitcode (0 by default). This method SHOULD be called by the
  script unless --timeout/--quit-on-load are in place.

**WebPage.print\(obj\)**
  Convenience method, adds obj to the resulting JSON object under the `print`
  attribute.

**WebPage.error\(obj\)**
  Convenience method, adds obj to the resulting JSON object under the `error`
  attribute.

**WebPage.result\(attrname, val\)**
  The generic way to add content to the result object is through this method,
  **val** must be serializable as JSON.

If your script raises an exception, the exception will be inserted into the
error object, and the worker will exit immediately.

# QUIRKS

Depending on what your are doing, it may be hard to load your scripts at the proper
time, in some cases they may run earlier than you want, and have to setup a handler
to be called at a later time (e.g. a timer or any other event). In those cases the
worker will not track the execution of the handler, i.e. if you raise we will
not catch it.

Since the worker is actually a Webkit browser it behaves exactly like one, it even
tries to play audio and video - we inject some Javascript overrides to prevent media
play from starting so be warned if you see some weird behaviour around audio/video
tags.

# EXIT STATUS

The exit status is 0 on success and -1 on error, unless the script specifies otherwise
when calling **WebPage.done()**.

# TODO

So many things - Cookie Jars, Custom headers, Capture HTTP requests

