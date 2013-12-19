#pragma once
#ifndef OPENALSYSTEM_H
#define OPENALSYSTEM_H

#include <AL/al.h>
#include <AL/alc.h>
#include <memory>
#include "resources/ALBuffer.h"

namespace Sigma {
	class OpenALSystem
	{
	public:
		OpenALSystem();

		/**
		 * \brief Starts the OpenAL audio system.
		 *
		 * Starts the OpenAL system and sets up an audio context.
		 * \return false on failure, true otherwise.
		 */
		bool Start();

		/**
		 * \brief Update listener position and handle queuing.
		 *
		 * Updates listener position and handles any queuing and/or streaming operations.
		 * \return true if any audio processing was performed
		 */
		bool Update();

		void test();
	private:
		std::unique_ptr<resource::ALBuffer> testbuff;
		ALuint testsource;

		ALCdevice* device;
		ALCcontext* context;

	}; // class OpenALSystem
} // namespace Sigma

#endif // OPENALSYSTEM_H