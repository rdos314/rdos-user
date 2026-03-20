/*#######################################################################
# RDOS operating system
# Copyright (C) 1988-2025, Leif Ekblad
#
# MIT License
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in all
# copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
# SOFTWARE.
#
# The author of this program may be contacted at leif@rdos.net
#
# section.h
# Critical section class
#
########################################################################*/

#ifndef _SECTION_H
#define _SECTION_H

#ifdef __RDOS__
#include "rdos.h"
#else
#include <pthread.h>
#endif

/**
 * @class TSection
 * @brief A synchronization utility for managing mutually exclusive access to shared resources.
 *
 * The TSection class provides functionality to ensure that critical sections of code
 * are accessed by only one thread at a time. It abstracts platform-specific synchronization
 * mechanisms, either using `pthread_mutex_t` on non-RDOS systems or `struct RdosFutex` on RDOS systems.
 *
 * The synchronization block is identified by a name with a maximum length of 32 characters.
 *
 * @note On RDOS systems, this class makes use of a Futex-based mechanism for synchronization.
 */
class TSection
{
 /**
  * Constructs a TSection object by initializing its name and appropriate synchronization mechanism.
  *
  * @param Name A null-terminated string specifying the name of the section.
  *             It is truncated to 32 characters if longer.
  * @return None
  */
public:
    TSection(const char *Name);

 /**
  * @brief Destructor for the TSection class.
  *
  * Cleans up resources associated with a TSection object. On systems
  * where __RDOS__ is defined, it resets the associated RdosFutex
  * to ensure proper cleanup.
  */
 ~TSection();

 /**
  * Acquires the section lock to ensure thread-safe access to the protected resource.
  *
  * This method blocks the calling thread if the lock is already held by another thread,
  * until the lock becomes available. It is designed to ensure mutual exclusion in
  * multi-threaded environments, providing a critical section where only one thread at
  * a time can execute.
  *
  * Implementation details vary depending on the underlying platform. On RDOS systems,
  * it uses a futex mechanism for synchronization. On POSIX-compliant systems, it
  * employs a pthread mutex for locking.
  *
  * Make sure to pair each call to `Enter` with a corresponding call to `Leave`
  * to release the lock and avoid deadlocks.
  */
 void Enter() const;

 /**
  * Releases the lock held by the current thread on the synchronization object.
  *
  * This method should be called after the thread has finished accessing the
  * shared resource protected by the synchronization object. It is responsible
  * for unlocking the mutex or futex, allowing other threads to proceed.
  *
  * On the RDOS platform, this method uses `RdosLeaveFutex` to release the futex.
  * On other platforms, the method uses `pthread_mutex_unlock` to unlock the mutex.
  *
  * It is important to ensure that every call to `Enter` is eventually matched
  * by a corresponding call to `Leave` to avoid deadlocks.
  */
 void Leave() const;

private:
#ifdef __RDOS__
 /**
  * @brief Represents a futex (fast user-space mutex) structure.
  *
  * The `RdosFutex` is used for synchronization at the user-space level to
  * avoid kernel transitions in certain operating systems (such as __RDOS__).
  * It is a lightweight mechanism designed to handle contention efficiently when
  * dealing with threads attempting to gain access to a shared resource.
  *
  * This structure is platform-specific and is typically enclosed as part of
  **/
 struct RdosFutex Futex;
#else
 /**
  * @brief Mutex used for thread synchronization.
  *
  * This mutex is used to control access to a shared resource in a multithreaded
  * environment, ensuring that only one thread can access the resource at a time.
  * It is part of the TSection class implementation on non-RDOS systems and is
  * initialized with `PTHREAD_MUTEX_INITIALIZER`. The mutex is locked and unlocked
  * using `pthread_mutex_lock` and `pthread_mutex_unlock` respectively to manage
  * critical sections.
  *
  * @note This variable is only available on systems that do not use RDOS.
  */
 mutable pthread_mutex_t Mutex;
#endif
 /**
  * @brief A fixed-size character array to store the name of a section.
  *
  * This variable is used to hold the name of a synchronization section up to 32 characters
  * long, with a null-terminating character at the end. It is initialized during the
  * construction of the TSection object and is used as a human-readable identifier in
  * certain system-specific operations.
  *
  * @note The size of the array is 33 to accommodate the null-terminator character, ensuring
  * that names with a maximum length of 32 characters are safely stored.
  */
 char FName[33];
};

#endif
