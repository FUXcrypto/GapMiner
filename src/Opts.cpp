/**
 * Implementation of GapMiners (simple) option parsing 
 *
 * Copyright (C)  2014  The Gapcoin developers  <info@gapcoin.org>
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#ifndef __STDC_FORMAT_MACROS 
#define __STDC_FORMAT_MACROS 
#endif
#ifndef __STDC_LIMIT_MACROS  
#define __STDC_LIMIT_MACROS  
#endif
#include "Opts.h"

/**
 * Returns if argv contains the given arg
 */
static char has_arg(int argc, 
                    char *argv[], 
                    const char *short_arg, 
                    const char *long_arg) {

  int i;
  for (i = 1; i < argc; i++) {
    if ((short_arg != NULL && !strcmp(argv[i], short_arg)) ||
        (long_arg != NULL  && !strcmp(argv[i], long_arg))) {
      
      return 1;
    }
  }

  return 0;
}

/**
 * Returns the given argument of arg
 */
static char *get_arg(int argc, 
                     char *argv[], 
                     const char *short_arg, 
                     const char *long_arg) {

  int i;
  for (i = 1; i < argc - 1; i++) {
    if ((short_arg != NULL && !strcmp(argv[i], short_arg)) ||
        (long_arg != NULL  && !strcmp(argv[i], long_arg))) {
      
      return argv[i + 1];
    }
  }

  return NULL;
}

/**
 * shorter macros 
 */
#define has_arg1(long_arg) has_arg(argc, argv, NULL, long_arg)
#define has_arg2(short_arg, long_arg) has_arg(argc, argv, short_arg, long_arg)
#define has_arg4(argc, argv, short_arg, long_arg) \
  has_arg(argc, argv, short_arg, long_arg)
#define has_argx(X, T1, T2, T3, T4, FUNC, ...) FUNC
#define has_arg(...) has_argx(, ##__VA_ARGS__,            \
                                has_arg4(__VA_ARGS__),    \
                                "3 args not allowed",     \
                                has_arg2(__VA_ARGS__),    \
                                has_arg1(__VA_ARGS__))

#define get_arg1(long_arg) get_arg(argc, argv, NULL, long_arg)
#define get_arg2(short_arg, long_arg) get_arg(argc, argv, short_arg, long_arg)
#define get_arg4(argc, argv, short_arg, long_arg) \
  get_arg(argc, argv, short_arg, long_arg)
#define get_argx(X, T1, T2, T3, T4, FUNC, ...) FUNC
#define get_arg(...) get_argx(, ##__VA_ARGS__,            \
                                get_arg4(__VA_ARGS__),    \
                                "3 args not allowed",     \
                                get_arg2(__VA_ARGS__),    \
                                get_arg1(__VA_ARGS__))

/**
 * to get integer args
 */
#define get_i_arg(...)                                          \
  (has_arg(__VA_ARGS__) ? atoi(get_arg(__VA_ARGS__)) : -1)

#define get_l_arg(...)                                          \
  (has_arg(__VA_ARGS__) ? atol(get_arg(__VA_ARGS__)) : -1)

#define get_ll_arg(...)                                         \
  (has_arg(__VA_ARGS__) ? atoll(get_arg(__VA_ARGS__)) : -1)

/**
 * to get float args
 */
#define get_f_arg(...)                                         \
  (has_arg(__VA_ARGS__) ? atof(get_arg(__VA_ARGS__)) : -1.0)


/* only instance of this */
Opts *Opts::only_instance = NULL;

/* synchronization mutexes */
pthread_mutex_t Opts::creation_mutex = PTHREAD_MUTEX_INITIALIZER;

/* initializes all possible args */
Opts::Opts(int argc, char *argv[]) :
host(     "-o", "--host",           "host ip address",                             true),
port(     "-p", "--port",           "port to connect to",                          true),
user(     "-u", "--user",           "user for gapcoin rpc authentification",       true),
pass(     "-x", "--pwd",            "password for gapcoin rpc authentification",   true),
quiet(    "-q", "--quiet",          "be quiet (only prints shares)",               false),
stats(    "-i", "--stats-interval", "interval (sec) to print mining informations", true),
threads(  "-t", "--threads",        "number of mining threads",                    true),
pull(     "-l", "--pull-interval",  "seconds to wait between getwork request",     true),
timeout(  "-m", "--timeout",        "seconds to wait for server to respond",       true),
stratum(  "-r", "--stratum",        "use stratum protocol for connection",         false),
sievesize("-s", "--sieve-size",     "the prime sieve size",                        true),
primes(   "-r", "--sieve-primes",   "number of primes for sieving",                true),
shift(    "-f", "--shift",          "the adder shift",                             true),
help(     "-h", "--help",           "print this information",                      false),
license(  "-v", "--license",        "show license of this program",                false) {
       
  /* get command line opts */
  host.active = has_arg(host.short_opt,  host.long_opt);
  if (host.active)
    host.arg = get_arg(host.short_opt,  host.long_opt);
                                          
  port.active = has_arg(port.short_opt,  port.long_opt);
  if (port.active)
    port.arg = get_arg(port.short_opt,  port.long_opt);
                                          
  user.active = has_arg(user.short_opt,  user.long_opt);
  if (user.active)
    user.arg = get_arg(user.short_opt,  user.long_opt);
                                          
  pass.active = has_arg(pass.short_opt,  pass.long_opt);
  if (pass.active)
    pass.arg = get_arg(pass.short_opt,  pass.long_opt);

  quiet.active = has_arg(quiet.short_opt, quiet.long_opt);

  stats.active = has_arg(stats.short_opt, stats.long_opt);
  if (stats.active)
    stats.arg = get_arg(stats.short_opt, stats.long_opt);

  threads.active = has_arg(threads.short_opt, threads.long_opt);
  if (threads.active)
    threads.arg = get_arg(threads.short_opt, threads.long_opt);

  pull.active = has_arg(pull.short_opt,  pull.long_opt);
  if (pull.active)
    pull.arg = get_arg(pull.short_opt,  pull.long_opt);
                                          
  timeout.active = has_arg(timeout.short_opt,  timeout.long_opt);
  if (timeout.active)
    timeout.arg = get_arg(timeout.short_opt,  timeout.long_opt);

  stratum.active = has_arg(stratum.short_opt,  stratum.long_opt);
                                          
  sievesize.active = has_arg(sievesize.short_opt,  sievesize.long_opt);
  if (sievesize.active)
    sievesize.arg = get_arg(sievesize.short_opt,  sievesize.long_opt);
                                          
  primes.active = has_arg(primes.short_opt,  primes.long_opt);
  if (primes.active)
    primes.arg = get_arg(primes.short_opt,  primes.long_opt);
                                          
  shift.active = has_arg(shift.short_opt,  shift.long_opt);
  if (shift.active)
    shift.arg = get_arg(shift.short_opt,  shift.long_opt);
                                          
  help.active = has_arg(help.short_opt,  help.long_opt);
  license.active = has_arg(license.short_opt,  license.long_opt);
}

/* access or create the only instance of this */
Opts *Opts::get_instance(int argc, char *argv[]) {
  
  pthread_mutex_lock(&creation_mutex);

  /* allow only one creation */
  if (argc != 0 && argv != NULL && only_instance == NULL) {
    only_instance = new Opts(argc, argv);
  }

  pthread_mutex_unlock(&creation_mutex);

  return only_instance;
}


/* get help */
string Opts::get_help()  { 
  stringstream ss;

  ss << "  GapMiner  Copyright (C)  2014  The Gapcoin developers  <info@gapcoin.org>\n\n";
  ss << "Required Options:   \n\n";

  ss << "  " << host.short_opt  << "  " << left << setw(18);
  ss << host.long_opt << "  " << host.description << "\n\n";

  ss << "  " << port.short_opt << "  " << left << setw(18);
  ss << port.long_opt << "  " << port.description << "\n\n";

  ss << "  " << user.short_opt << "  " << left << setw(18);
  ss << user.long_opt << "  " << user.description << "\n\n";

  ss << "  " << pass.short_opt << "  " << left << setw(18);
  ss << pass.long_opt << "  " << pass.description << "\n\n";

  ss << "Additional Options:\n\n";

  ss << "  " << quiet.short_opt << "  " << left << setw(18);
  ss << quiet.long_opt << "  " << quiet.description << "\n\n";

  ss << "  " << stats.short_opt << "  " << left << setw(18);
  ss << stats.long_opt << "  " << stats.description << "\n\n";

  ss << "  " << threads.short_opt << "  " << left << setw(18);
  ss << threads.long_opt << "  " << threads.description << "\n\n";

  ss << "  " << pull.short_opt  << "  " << left << setw(18);
  ss << pull.long_opt << "  " << pull.description << "\n\n";

  ss << "  " << timeout.short_opt  << "  " << left << setw(18);
  ss << timeout.long_opt << "  " << timeout.description << "\n\n";

  ss << "  " << stratum.short_opt  << "  " << left << setw(18);
  ss << stratum.long_opt << "  " << stratum.description << "\n\n";

  ss << "  " << sievesize.short_opt  << "  " << left << setw(18);
  ss << sievesize.long_opt << "  " << sievesize.description << "\n\n";

  ss << "  " << primes.short_opt  << "  " << left << setw(18);
  ss << primes.long_opt << "  " << primes.description << "\n\n";

  ss << "  " << shift.short_opt  << "  " << left << setw(18);
  ss << shift.long_opt << "  " << shift.description << "\n\n";

  ss << "  " << help.short_opt << "  " << left << setw(18);
  ss << help.long_opt << "  " << help.description << "\n\n";

  ss << "  " << license.short_opt << "  " << left << setw(18);
  ss << license.long_opt << "  " << license.description << "\n\n";

  return ss.str();
}
