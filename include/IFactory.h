#pragma once
#ifndef IFACTORY_H
#define IFACTORY_H

#include <functional>
#include <vector>
#include <map>
#include "Property.h"
#include "IComponent.h"
#include "Sigma.h"

namespace Sigma {
	class IFactory {
	public:
		typedef std::function<IComponent*(const id_t, const std::vector<Property>&)> FactoryFunction;
		IFactory(){};
		virtual ~IFactory(){};
		/**
		 * \brief Returns the list of Factory functions and types they create
		 *
		 * \return std::map<std::string, FactoryFunction> Contains Callbacks for different Component types that can be created by this class
		 */
		virtual std::map<std::string,IFactory::FactoryFunction> getFactoryFunctions() = 0;
	protected:
	private:
	};
}

#endif // IFACTORY_H
