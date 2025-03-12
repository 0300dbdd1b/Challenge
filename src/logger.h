
#ifndef LOGGER_H
#define LOGGER_H

/* Include standard headers */
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <errno.h>
#include <time.h>

/* Platform-specific includes and definitions */
#ifdef _WIN32
    #include <windows.h>
    #include <io.h>
    #define _STRDUP _strdup
    #define _LOCALTIME(t, tm) localtime_s(&(tm), &(t))
#else
    #include <unistd.h>
    #define _STRDUP strdup
    #define _LOCALTIME(t, tm) (tm = *localtime(&(t)))
#endif

/* Log levels */
#define LOG_LEVEL_DEBUG 0
#define LOG_LEVEL_INFO  1
#define LOG_LEVEL_WARN  2
#define LOG_LEVEL_ERROR 3
#define LOG_LEVEL_FATAL 4

/* Default log level */
#ifndef LOG_LEVEL
    #define LOG_LEVEL LOG_LEVEL_DEBUG
#endif

/* Log file path and mode defaults */
#ifndef LOG_FILE_PATH
    #define LOG_FILE_PATH "default_log.log"
#endif

#ifndef LOG_FILE_MODE
    #define LOG_FILE_MODE "a"
#endif

#define MAX_LOG_MESSAGE_LENGTH 2048
#define LOG_TIME_FORMAT "%Y-%m-%d %H:%M:%S"

/* Logging modes */
typedef enum {
    LOG_MODE_FILE,
    LOG_MODE_STDERR,
    LOG_MODE_STDOUT
} LogMode;

/* Global settings */
static int _log_level = LOG_LEVEL;
static FILE* _log_output_stream = NULL;
static char _log_file_path[256] = LOG_FILE_PATH;
static char _log_file_mode[5] = LOG_FILE_MODE;
static LogMode _log_mode = LOG_MODE_STDERR;
static int _use_colors = 1;
static int _show_file_line = 1;
static int _show_timestamp = 1;
static int _show_log_level = 1;  /* Toggle log level visibility */

/* ANSI color macros */

// Text colors
#define COLOR_WHITE     "\033[37m"
#define COLOR_GREEN     "\033[32m"
#define COLOR_YELLOW    "\033[33m"
#define COLOR_RED       "\033[31m"
#define COLOR_MAGENTA   "\033[35m"
#define COLOR_CYAN      "\033[36m"
#define COLOR_BLUE      "\033[34m"


#define COLOR_BLACK      "\033[30m"
#define COLOR_DARK_GRAY  "\033[90m"
#define COLOR_LIGHT_RED  "\033[91m"
#define COLOR_LIGHT_GREEN "\033[92m"
#define COLOR_LIGHT_YELLOW "\033[93m"
#define COLOR_LIGHT_BLUE "\033[94m"
#define COLOR_LIGHT_MAGENTA "\033[95m"
#define COLOR_LIGHT_CYAN "\033[96m"
#define COLOR_LIGHT_WHITE "\033[97m"

// Background colors
#define BG_BLACK   "\033[40m"
#define BG_RED     "\033[41m"
#define BG_GREEN   "\033[42m"
#define BG_YELLOW  "\033[43m"
#define BG_BLUE    "\033[44m"
#define BG_MAGENTA "\033[45m"
#define BG_CYAN    "\033[46m"
#define BG_WHITE   "\033[47m"

#define BG_DARK_GRAY  "\033[100m"
#define BG_LIGHT_RED  "\033[101m"
#define BG_LIGHT_GREEN "\033[102m"
#define BG_LIGHT_YELLOW "\033[103m"
#define BG_LIGHT_BLUE "\033[104m"
#define BG_LIGHT_MAGENTA "\033[105m"
#define BG_LIGHT_CYAN "\033[106m"
#define BG_LIGHT_WHITE "\033[107m"

// Text styles
#define STYLE_BOLD       "\033[1m"
#define STYLE_DIM        "\033[2m"
#define STYLE_UNDERLINE  "\033[4m"
#define STYLE_BLINK      "\033[5m"
#define STYLE_REVERSE    "\033[7m"
#define STYLE_HIDDEN     "\033[8m"

// Reset all
#define COLOR_RESET      "\033[0m"

#ifdef _WIN32
    static WORD _log_colors[] = {7, 10, 14, 12, 4};
#else
    static const char* _log_colors[] = {
        COLOR_WHITE,   /* DEBUG */
        COLOR_GREEN,   /* INFO */
        COLOR_YELLOW,  /* WARN */
        COLOR_RED,     /* ERROR */
        COLOR_MAGENTA  /* FATAL */
    };
#endif

/* Function to toggle log level visibility */
static inline void SetShowLogLevel(int enable) { _show_log_level = enable; }
static inline void SetUseColors(int enable) { _use_colors = enable; }
static inline void SetShowFileLine(int enable) { _show_file_line = enable; }
static inline void SetShowTimestamp(int enable) { _show_timestamp = enable; }

/* Change color for a specific log level */
static inline void SetLogColor(int level, const char* color_code) {
#ifndef _WIN32
    if (level >= LOG_LEVEL_DEBUG && level <= LOG_LEVEL_FATAL) {
        _log_colors[level] = color_code;
    }
#endif
}

/* Convert log level to string */
static inline const char* _log_level_to_string(int level) {
    switch (level) {
        case LOG_LEVEL_DEBUG: return "DEBUG";
        case LOG_LEVEL_INFO:  return "INFO";
        case LOG_LEVEL_WARN:  return "WARN";
        case LOG_LEVEL_ERROR: return "ERROR";
        case LOG_LEVEL_FATAL: return "FATAL";
        default:              return "UNKNOWN";
    }
}

/* Get current time string */
static inline void _current_time_string(char* buffer, size_t size) {
    time_t now = time(NULL);
    struct tm tinfo;
    _LOCALTIME(now, tinfo);
    strftime(buffer, size, LOG_TIME_FORMAT, &tinfo);
}

/* Open the log file */
static inline void _open_log_file() {
    if (_log_output_stream) fclose(_log_output_stream);
    _log_output_stream = fopen(_log_file_path, _log_file_mode);
}

/* Set log mode */
static inline void _set_log_mode(LogMode mode) {
    _log_mode = mode;
    if (_log_mode == LOG_MODE_FILE) {
        _open_log_file();
    }
}

/* Set log level */
static inline void _set_log_level(int level) { _log_level = level; }

/* Set log file path */
static inline void _set_log_file_path(const char* filepath) {
    strncpy(_log_file_path, filepath, sizeof(_log_file_path) - 1);
    _log_file_path[sizeof(_log_file_path) - 1] = '\0';
    if (_log_mode == LOG_MODE_FILE) _open_log_file();
}

/* Clear log file */
static inline void _clear_log_file() {
    if (_log_mode == LOG_MODE_FILE && _log_output_stream) {
        freopen(_log_file_path, "w", _log_output_stream);
    }
}

/* Erase log file */
static inline void _erase_log_file() {
    if (_log_mode == LOG_MODE_FILE) {
        fclose(_log_output_stream);
        remove(_log_file_path);
        _open_log_file();
    }
}

/* Log a message */
static inline void _log_message(int level, const char* file, int line, const char* format, ...) {
    if (level >= _log_level) {
        char log_message[MAX_LOG_MESSAGE_LENGTH];
        char time_buffer[64] = "";

        if (_show_timestamp) {
            _current_time_string(time_buffer, sizeof(time_buffer));
        }

        va_list args;
        va_start(args, format);
        vsnprintf(log_message, sizeof(log_message), format, args);
        va_end(args);

        FILE* output_stream = (_log_mode == LOG_MODE_FILE) ? _log_output_stream :
                              (_log_mode == LOG_MODE_STDERR) ? stderr : stdout;

        if (_use_colors && _log_mode != LOG_MODE_FILE) {
#ifndef _WIN32
            fprintf(output_stream, "%s", _log_colors[level]);
#endif
        }

        if (_show_timestamp) fprintf(output_stream, "[%s] ", time_buffer);
        if (_show_file_line) fprintf(output_stream, "%s:%d ", file, line);
        if (_show_log_level) fprintf(output_stream, "[%s] ", _log_level_to_string(level));

        fprintf(output_stream, "%s\n", log_message);
        fflush(output_stream);

        if (_use_colors && _log_mode != LOG_MODE_FILE) {
#ifndef _WIN32
            fprintf(output_stream, "%s", COLOR_RESET);
#endif
        }
    }
}

/* Logging macros */
#define LOG_DEBUG(...) _log_message(LOG_LEVEL_DEBUG, __FILE__, __LINE__, __VA_ARGS__)
#define LOG_INFO(...)  _log_message(LOG_LEVEL_INFO, __FILE__, __LINE__, __VA_ARGS__)
#define LOG_WARN(...)  _log_message(LOG_LEVEL_WARN, __FILE__, __LINE__, __VA_ARGS__)
#define LOG_ERROR(...) _log_message(LOG_LEVEL_ERROR, __FILE__, __LINE__, __VA_ARGS__)
#define LOG_FATAL(...) _log_message(LOG_LEVEL_FATAL, __FILE__, __LINE__, __VA_ARGS__)

/* Fully restored macros */
#define SET_LOG_LEVEL(level) _set_log_level(level)
#define SET_LOG_FILE_PATH(filepath) _set_log_file_path(filepath)
#define SET_LOG_MODE(mode) _set_log_mode(mode)
#define CLEAR_LOG_FILE() _clear_log_file()
#define ERASE_LOG_FILE() _erase_log_file()
#define SET_LOG_SHOW_LEVEL(enable) SetShowLogLevel(enable)
#define SET_LOG_USE_COLORS(enable) SetUseColors(enable)
#define SET_LOG_SHOW_FILE_LINE(enable) SetShowFileLine(enable)
#define SET_LOG_SHOW_TIMESTAMP(enable) SetShowTimestamp(enable)

#endif /* LOGGER_H */

