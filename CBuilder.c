#if 0
cc CBuilder.c -o /tmp/CBuilder
/tmp/CBuilder
exit
#endif


/*
 * CBuild V0.0.2
 */

#ifndef		_CBUILD_
	#define	_CBUILD_
	#define	BUILDER_SOURCE		"CBuilder.c"
	#define	BUILDER_BASE_NAME	"CBuilder"

	#if defined(_WIN32) || defined(_WIN64)
		#include <windows.h>
		#include <direct.h>
		#ifndef		PLATFORM_WINDOWS
			#	define	PLATFORM_WINDOWS
    		#	define mkdir _mkdir
		#endif
		#define BUILDER_NAME			BUILDER_BASE_NAME ".exe"
		#define BUILDER_CC				"msvc"
		#define BUILDER_CFLAGS			""
		#define BUILDER_LDFLAGS			""
		#define BUILDER_DEBUG_CFLAGS	"-g3"
		#define BUILDER_DEBUG_LDFLAGS	""
	#elif defined(__linux__) || defined(__APPLE__) && defined(__MACH__)
		#ifndef		PLATFORM_UNIX
			#define	PLATFORM_UNIX
		#endif
		#define BUILDER_NAME	"./" BUILDER_BASE_NAME
		#include <sys/types.h>
		#include <sys/wait.h>
		#include <sys/stat.h>
		#include <unistd.h>
		#include <time.h>
		#include <stdint.h>
		#include <stdio.h>
		#include <stdlib.h>
		#include <string.h> 
		#include <unistd.h>
		#include <stdarg.h>
		#include <errno.h>
		#include <stddef.h>
		#include <pthread.h>

		#if defined(__linux__)
			#ifndef		PLATFORM_LINUX
				#define	PLATFORM_LINUX
			#endif
			#define		BUILDER_CC				"cc"
			#define		BUILDER_CFLAGS			""
			#define		BUILDER_LDFLAGS			""
			#define		BUILDER_DEBUG_CFLAGS	"-g3"
			#define		BUILDER_DEBUG_LDFLAGS	""
		#elif defined(__APPLE__) && defined(__MACH__)
			#ifndef		PLATFORM_MACOS
				#define	PLATFORM_MACOS
			#endif
			#define		BUILDER_CC				"clang"
			#define		BUILDER_CFLAGS			""
			#define		BUILDER_LDFLAGS			""
			#define		BUILDER_DEBUG_CFLAGS	"-g3"
			#define		BUILDER_DEBUG_LDFLAGS	""
		#else
			#error "Unsupported platform"
		#endif
	#endif

#define		DEFAULT_STRING_ALLOCATION_SIZE	500
#define		MAX_RULES						100
#define		MAX_COMMAND						300













/*								This is a copy paste of my logger V2			*/

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











typedef struct String
{
	char *c_str;              // Pointer to the character array
	unsigned int length;      // Current length of the string
	unsigned int size;        // Allocated size of the string buffer
} String;
typedef String Command;


String *CreateStringEmpty(void)
{
	String *string = (String *)malloc(sizeof(String));
	if (!string) return NULL;

	string->c_str = (char *)malloc(DEFAULT_STRING_ALLOCATION_SIZE * sizeof(char));
	if (!string->c_str)
	{
		free(string);
		return NULL;
	}

	string->length = 0;
	string->size = DEFAULT_STRING_ALLOCATION_SIZE;
	string->c_str[0] = '\0';
	return string;
}

void AppendToString(String *str, const char *element)
{
	if (!str || !element) return;
	unsigned long elementLength = strlen(element);
	unsigned long requiredSize = str->length + elementLength + 1;
	if (requiredSize > str->size)
	{
		unsigned int newSize = str->size * 2;
		while (newSize < requiredSize)
			newSize *= 2;
		char *newBuffer = (char *)realloc(str->c_str, newSize);
		if (!newBuffer) return;
		str->c_str = newBuffer;
		str->size = newSize;
	}
	memcpy(str->c_str + str->length, element, elementLength);
	str->length += elementLength;
	str->c_str[str->length] = '\0';
}

void AppendToStringVA(String *str, va_list args)
{
	if (!str) return;

	const char *arg = va_arg(args, const char *);
	while (arg != NULL)
	{
		if (str->length > 0)
		{
			AppendToString(str, " ");
		}
		AppendToString(str, arg);
		arg = va_arg(args, const char *);
	}
}

void AppendToStringNULL(String *str, ...)
{
	if (!str) return;
	va_list args;
	va_start(args, str);
	AppendToStringVA(str, args);
	va_end(args);
}

// Implementation function that handles variadic arguments
String *CreateStringImpl(int dummy, ...)
{
	String *string = CreateStringEmpty();
	if (!string) return NULL;

	// Process strings, if any
	va_list args;
	va_start(args, dummy);
	AppendToStringVA(string, args);
	va_end(args);

	return string;
}

// WARN: It does not look like the proper way to handle that.
#define CreateString(...) CreateStringImpl(0, ##__VA_ARGS__, NULL)

#define CreateCommand CreateString
#define AppendToCommandNULL AppendToStringNULL
#define AppendToCommand(command, ...) AppendToCommandNULL(command, __VA_ARGS__, NULL)




typedef void(*RuleFunction)(void);
typedef struct Rule
{
	char			*name;
	RuleFunction	function;
} Rule;

Rule gRules[MAX_RULES];
int	gRuleCount;


void CreateRule(const char *ruleName, RuleFunction function) {
	if (gRuleCount >= MAX_RULES)
		return;
	gRules[gRuleCount].name = strdup(ruleName);
	gRules[gRuleCount].function = function;
	gRuleCount++;
}

void HelpRule(void)
{
	LOG_INFO(COLOR_BLUE"Available rules:"COLOR_RESET);
	for (int i = 0; i < gRuleCount; i++)
		LOG_INFO("- "COLOR_CYAN"%s"COLOR_RESET, gRules[i]);
}

int ExecuteRule(const char *rule)
{
	for (int i = 0; i < gRuleCount; i++)
	{
		if (strcmp(gRules[i].name, rule) == 0)
		{
			LOG_INFO(COLOR_YELLOW"Executing rule "COLOR_CYAN"%s"COLOR_RESET, gRules[i].name);
			gRules[i].function();
			return 1;
		}
	}
	LOG_WARN(COLOR_RED"Rule not found: "COLOR_CYAN"%s"COLOR_RESET, rule);
	HelpRule();
	return 0;
}


int ExecuteCommandGeneric(const Command *command, int runInThread) {
	if (!command || !command->c_str) {
		LOG_ERROR("Invalid command: NULL pointer");
		return 0;
	}
	LOG_INFO(COLOR_YELLOW"--cmd--"COLOR_RESET" %s", command->c_str);

#ifdef PLATFORM_WINDOWS
	STARTUPINFO si;
	PROCESS_INFORMATION pi;
	ZeroMemory(&si, sizeof(si));
	si.cb = sizeof(si);
	ZeroMemory(&pi, sizeof(pi));

	char *cmdBuffer = strdup(command->c_str);
	if (!cmdBuffer) {
		fprintf(stderr, "Memory allocation failed for command buffer\n");
		return 0;
	}

	if (!CreateProcess(NULL, cmdBuffer, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi)) {
		fprintf(stderr, "Command execution failed: %lu\n", GetLastError());
		free(cmdBuffer);
		return 0;
	}

	if (runInThread) {
		HANDLE thread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)WaitForSingleObject, pi.hProcess, 0, NULL);
		if (thread == NULL) {
			fprintf(stderr, "Failed to create thread: %lu\n", GetLastError());
			CloseHandle(pi.hProcess);
			CloseHandle(pi.hThread);
			free(cmdBuffer);
			return 0;
		}
		WaitForSingleObject(thread, INFINITE);
		CloseHandle(thread);
	} else {
		WaitForSingleObject(pi.hProcess, INFINITE);
	}

	CloseHandle(pi.hProcess);
	CloseHandle(pi.hThread);
	free(cmdBuffer);
#elif defined(PLATFORM_UNIX)
	pid_t pid = fork();
	if (pid < 0) {
		perror("Fork failed");
		return 0;
	}

	if (pid == 0) {
		execl("/bin/sh", "sh", "-c", command->c_str, (char *)NULL);
		perror("execl failed");
		exit(EXIT_FAILURE);
	} else {
		if (runInThread) {
			pthread_t waitThread;
			pthread_create(&waitThread, NULL, (void *(*)(void *))waitpid, (void *)&pid);
			pthread_join(waitThread, NULL);
		} else {
			int status;
			waitpid(pid, &status, 0);
			return WIFEXITED(status) ? (WEXITSTATUS(status) == 0 ? 1 : 0) : 0;
		}
	}
#endif

	return 1;
}
#define ExecuteCommand(command) ExecuteCommandGeneric(command, 0)
#define ExecuteCommandWithThreading(command) ExecuteCommandGeneric(command, 1)

void ExecuteCommands(int runInThread, ...)
{
	va_list args;
	va_start(args, runInThread);

	pthread_t *threads = NULL;
	int threadCount = 0;
	if (runInThread)
		threads = (pthread_t *)malloc(sizeof(pthread_t) * MAX_COMMAND); // Arbitrary maximum command count

	Command *cmd;
	while ((cmd = va_arg(args, Command *)) != NULL)
	{
		if (runInThread)
		{
			pthread_create(&threads[threadCount], NULL, (void *(*)(void *))ExecuteCommandGeneric, (void *)cmd);
			threadCount++; }
		else
		ExecuteCommand(cmd);
	}

	if (runInThread) {
		for (int i = 0; i < threadCount; i++) {
			pthread_join(threads[i], NULL);
		}
		free(threads);
	}

	va_end(args);
}
#define ExecuteCommands(...) ExecuteCommands(__VA_ARGS__, NULL)

time_t GetFileLastEditTime(const char *filePath)
{
	struct stat fileStat;
	if (stat(filePath, &fileStat) != 0)
	{
		perror("Failed to get file stats");
		return 0;
	}
	return fileStat.st_mtime;
}

void RebuildSelf(void)
{
	Command *selfBuildCmd = CreateCommand();
	AppendToCommand(selfBuildCmd, BUILDER_CC, BUILDER_CFLAGS, BUILDER_SOURCE, "-o", BUILDER_NAME);
	LOG_INFO(COLOR_YELLOW"Rebuilding CBuilder..."COLOR_RESET);
	if (!ExecuteCommand(selfBuildCmd))
	{
		LOG_FATAL("Failed to rebuild CBuilder");
		exit(EXIT_FAILURE);
	}
	LOG_INFO(COLOR_GREEN"CBuilder rebuilt successfully."COLOR_RESET);
	free(selfBuildCmd->c_str);
	free(selfBuildCmd);
}

void RunSelf(String *args)
{
	Command *cmd = CreateCommand();
#ifdef PLATFORM_WINDOWS
	AppendToCommand(cmd, BUILDER_NAME);
#else
	AppendToCommand(cmd, BUILDER_NAME);
#endif
	AppendToCommand(cmd, args->c_str);
	if (!ExecuteCommand(cmd))
		LOG_FATAL("Failed to execute command %s", cmd->c_str);
}

void ManageRebuild(int argc, char **argv)
{
	time_t sourceEditTime = GetFileLastEditTime(BUILDER_SOURCE);
	time_t execEditTime = GetFileLastEditTime(BUILDER_NAME);

	if (sourceEditTime > execEditTime)
	{
		LOG_INFO(COLOR_YELLOW"Source file is newer than executable. Rebuilding..."COLOR_RESET);
		String *args = CreateString();
		for (int i = 1; i < argc; i++)
			AppendToCommand(args, argv[i]);
		RebuildSelf();
		RunSelf(args);
		exit(0);
	}
}

void BuildDebugSelf(void)
{
	Command *cmd = CreateCommand(BUILDER_CC, BUILDER_DEBUG_CFLAGS, BUILDER_LDFLAGS, BUILDER_SOURCE, "-o", BUILDER_NAME);
	ExecuteCommand(cmd);
}

void ManageRules(int argc, char **argv)
{
	int retValue;
	ManageRebuild(argc, argv);
	for (int i = 1; i < argc; i++)
		retValue = ExecuteRule(argv[i]);
	if (argc == 1)
	{
		LOG_INFO(COLOR_BLUE"No rule provided -- using the first rule declared"COLOR_RESET);
		LOG_INFO(COLOR_YELLOW"Executing rule "COLOR_CYAN"%s"COLOR_RESET, gRules[0].name);
		gRules[0].function();
	}
}

/* ----------------------------------------- SCRIPT UTILS ----------------------------------------- */

int FileExists(const char *path)
{
	struct stat buffer;
	return (stat(path, &buffer) == 0);
}

int CreateFile(const char *path, const char *content)
{
	FILE *file = fopen(path, "w");
	if (!file) return 0;

	if (content && *content) {
		fprintf(file, "%s", content);
	}

	fclose(file);
	return 1;
}

int CreateFileWithMode(const char *path, const char *content, const char *mode)
{
	FILE *file = fopen(path, mode ? mode : "w");
	if (!file) return 0;

	if (content && *content) {
		fprintf(file, "%s", content);
	}

	fclose(file);
	return 1;
}

int CopyFile(const char *src, const char *dest)
{
	FILE *srcFile = fopen(src, "rb");
	if (!srcFile) return 0;

	FILE *destFile = fopen(dest, "wb");
	if (!destFile) {
		fclose(srcFile);
		return 0;
	}

	char buffer[4096];
	size_t bytes;
	while ((bytes = fread(buffer, 1, sizeof(buffer), srcFile)) > 0) {
		fwrite(buffer, 1, bytes, destFile);
	}

	fclose(srcFile);
	fclose(destFile);
	return 1;
}

int RemoveFile(const char *path)
{
	return remove(path) == 0;
}

int CreateDirectory(const char *path)
{
	return mkdir(path, 0777) == 0 || errno == EEXIST;
}

int RemoveDirectory(const char *path)
{
#ifdef PLATFORM_WINDOWS
	return _rmdir(path) == 0;
#else
	return rmdir(path) == 0;
#endif
}

int DirectoryExists(const char *path) {
	struct stat buffer;
	return (stat(path, &buffer) == 0 && S_ISDIR(buffer.st_mode));
}





















/*
 * CBuilder v0.0.2
 * 
 * Description:
 * ------------
 * CBuilder is a lightweight, single-file build system written in C, designed for flexibility
 * and direct scriptability. It does not check for memory allocation failures, does not free memory,
 * and remains entirely contained within a single source file.
 *
 * How to Use:
 * -----------
 * - Modify the `main` function at the bottom of the file to add your custom rules.
 * - A rule is a function `void MyRule(void)` registered with `CreateRule("name", MyRule);`.
 * - Use `ExecuteCommand(CreateCommand("your_command_here"));` to execute shell commands.
 * - Use `ExecuteCommands(cmd1, cmd2);` to execute multiple commands.
 * - Run `./CBuilder help` to see the available rules.
 *
 * ----------------------------------------- USER SPACE ----------------------------------------- */

#if defined(PLATFORM_MACOS)
	#define EXECNAME	"Game"
	#define CC			"cc -fsanitize=address"
	#define CFLAGS		"-Wall -Wextra"

	#define RAYLIB_DIR				"src/extern/raylib-5.5/src/"
	#define	RAYLIB_BIN_NAME			"libraylib.a"
	#define RAYLIB_BIN_PATH			RAYLIB_DIR RAYLIB_BIN_NAME
	#define RAYLIB_BUILD_COMMAND	"cd "RAYLIB_DIR" && make PLATFORM=PLATFORM_DESKTOP"

	#define FFTW_DIR				"src/extern/fftw-3.3.10/"
	#define FFTW_BIN_NAME			"libfftw3.a"
	#define FFTW_BIN_PATH			FFTW_DIR".libs/"FFTW_BIN_NAME
	#define FFTW_BUILD_COMMAND		"cd "FFTW_DIR" && ./configure && make install"

	#define	MACOS_FRAMEWORKS	"-framework CoreVideo		\
								-framework CoreAudio		\
								-framework Cocoa,			\
								-framework IOKit,			\
								-framework GLUT,			\
								-framework OpenGL,			\
								-framework GameController,	\
								-framework AudioToolbox,	\
								-framework Metal,			\
								-framework CoreHaptics,		\
								-framework ForceFeedback,	\
								-framework Carbon,			\
								-framework AVFoundation"

	#define LDFLAGS		"-I"RAYLIB_DIR" -I"FFTW_DIR"/api/ "MACOS_FRAMEWORKS
#elif defined(PLATFORM_LINUX)
	#define EXECNAME	"Game"
	#define CC			"cc"
	#define CFLAGS		"-Wall -Wextra"

	#define RAYLIB_DIR				"src/extern/raylib-5.5/src/"
	#define	RAYLIB_BIN_NAME			"libraylib.a"
	#define RAYLIB_BIN_PATH			RAYLIB_DIR RAYLIB_BIN_NAME
	#define RAYLIB_BUILD_COMMAND	"cd "RAYLIB_DIR" && make PLATFORM_DESKTOP"

	#define LDFLAGS		"-I"RAYLIB_DIR

#elif defined(PLATFORM_WINDOWS)
	#define EXECNAME	"TiletimeMaker"
	#define CC			"msvc"
#endif


void BuildRaylib(void)
{
	LOG_INFO(COLOR_YELLOW"Building raylib..."COLOR_RESET);
	Command *cmd = CreateCommand(RAYLIB_BUILD_COMMAND);
	if (!ExecuteCommand(cmd))
		LOG_ERROR("Failed to build"RAYLIB_BIN_PATH);
	else
		LOG_INFO(COLOR_GREEN""RAYLIB_BIN_PATH"successfully built!"COLOR_RESET);
}

void BuildFFTW(void)
{
	LOG_INFO(COLOR_YELLOW"Building fftw..."COLOR_RESET);
	Command *cmd = CreateCommand(FFTW_BUILD_COMMAND);
	if (!ExecuteCommand(cmd))
		LOG_ERROR("Failed to build"FFTW_BIN_PATH);
	else
		LOG_INFO(COLOR_GREEN""FFTW_BIN_PATH"successfully built!"COLOR_RESET);
}

void BuildRule(void)
{
	if (!FileExists(RAYLIB_BIN_PATH))
	{
		LOG_INFO(COLOR_YELLOW""RAYLIB_BIN_NAME" not found"COLOR_RESET);
		BuildRaylib();
	}
	else
		LOG_INFO(COLOR_GREEN""RAYLIB_BIN_NAME" already exists"COLOR_RESET);
	if (!FileExists(FFTW_BIN_PATH))
	{
		LOG_INFO(COLOR_YELLOW""FFTW_BIN_NAME" not found"COLOR_RESET);
		BuildFFTW();
	}
	else
		LOG_INFO(COLOR_GREEN""FFTW_BIN_NAME" already exists"COLOR_RESET);
	Command *cmd = CreateCommand(CC, CFLAGS, LDFLAGS, RAYLIB_BIN_PATH, FFTW_BIN_PATH);
	AppendToCommand(cmd, "src/*.c");
	AppendToCommand(cmd, "-o", EXECNAME);
	if (!ExecuteCommand(cmd))
		LOG_ERROR("Failed to build "EXECNAME);
	else
		LOG_INFO(COLOR_GREEN""EXECNAME" successfully built!"COLOR_RESET);
}

void ExecRule(void)
{
	Command *execCommand = CreateCommand();
	AppendToCommand(execCommand, EXECNAME);
	ExecuteCommand(execCommand);
}

int main(int argc, char **argv)
{
	SET_LOG_SHOW_FILE_LINE(0);	// NOTE: Set to 1 to display the line number when logging.
	SET_LOG_SHOW_TIMESTAMP(0);	// NOTE: Set to 1 to display the timestamp when logging.
	SET_LOG_SHOW_LEVEL(0);		// NOTE: Set to 1 to display the log level when logging.

	LOG_INFO(COLOR_YELLOW"======== CBuilder v0.0.2 ========"COLOR_RESET);

	CreateRule("build", BuildRule);
	CreateRule("exec", ExecRule);

	CreateRule("build-raylib", BuildRaylib);
	CreateRule("build-fftw", BuildFFTW);

	CreateRule("help", HelpRule);
	CreateRule("self-rebuild", RebuildSelf);
	CreateRule("self-build-debug", BuildDebugSelf);

	ManageRules(argc, argv);
	LOG_INFO(COLOR_YELLOW"================================"COLOR_RESET);
	return 0;
}
#endif
