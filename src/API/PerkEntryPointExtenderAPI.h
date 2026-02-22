#pragma once

#include <type_traits>


#ifdef UNICODE
#define PEPE_API_SOURCE L"PerkEntryPointExtender.dll"
#else
#define PEPE_API_SOURCE "PerkEntryPointExtender.dll"
#endif


namespace RE
{
	using PerkEntryPoint = BGSPerkEntry::EntryPoint;
}

namespace PEPE
{
	//TODO:Want something to print the errors for RequestResult
	//TODO:Move RequestResult to API
	enum struct RequestResult
	{
		Success = -1,	//Success entry point usage.
		InvalidAPI,		//API failure, interface is not detected.
		Unsupported,	//entry point is not/no longer supported.
		UnmatchedArgs,	//entry arg number doesn't match no. args given
		BadFormArg,		//Invalid forms given as args
		BadEntryPoint,	//Invalid entry point value
		BadString,		//Invalid entry point name
		NoActor,		//No perk owner
		InvalidActor,	//Perk entry invalid on target
		InvalidOut,		//Doesn't have an out when it should
		UnknownFlags,	//Unrecognized flags used
		BadExtraData,	//Used ExtraData when giving an object reference (it has an extra data already)


		UnknownError//The last entry, if this is seen the API has an issue that's out of date to handle.

	};

	constexpr std::string_view inactiveCategory = "[:INACTIVE:]";

	struct Scope_EntryPointFlag
	{
		enum Enum : uint64_t
		{
			None = 0,
			ReverseOrder = 1 << 0,				//Reverses the priority of the perk entries being read in
			PRIVATE_UsesCollection = 1 << 1,	//PRIVATE: used to designate the form collection struct being used NOT to say that the out is a vector.

			Paired = 1 << 2,					//Use means the argument count is doubled, with the lower count coresponding with the upper count.
												// use pairs to enable automatically




			PRIVATE_Last,	//Does nothing but helps see what last unused flag is.
			PRIVATE_Total = (PRIVATE_Last - 1) << 1,//Flags after or equal to this will be seen as invalid

		};
	};

	using EntryPointFlag = Scope_EntryPointFlag::Enum;

	struct IFormCollection
	{
		//returns true or false if it's the right form loaded or not.
		virtual bool LoadForm(RE::TESForm* form) = 0;
	};


	template <typename T>
	struct BasicFormCollection
	{
		//If a non-collection is given
		BasicFormCollection(T& it) {}
	};

	template <typename T> requires (std::is_pointer_v<typename T::value_type> &&
		std::derived_from<std::remove_pointer_t<typename T::value_type>, RE::TESForm> &&
		requires(T t1, T::value_type t2) { t1.push_back(t2); })
	struct BasicFormCollection<T> : public IFormCollection
	{
		BasicFormCollection(T& it) : out{ it } {}


		bool LoadForm(RE::TESForm* form) override
		{
			if (auto it = form->As<std::remove_pointer_t<typename T::value_type>>()) {
				out.push_back(it);
				return true;
			}

			return false;
		}

		T& out;

	};


	struct Item
	{
		RE::TESBoundObject* object{};
		RE::ExtraDataList* extraData{};
	};

	//Move this.
	namespace detail
	{
		template <typename T>
		concept item_object = std::same_as<T, std::nullptr_t> ||
			(std::is_pointer_v<T> && std::derived_from<std::remove_pointer_t<T>, RE::TESForm>) ||
			(std::is_pointer_v<T> && std::same_as<std::remove_pointer_t<T>, RE::InventoryEntryData>) ||
			(!std::is_pointer_v<T> && std::same_as<T, PEPE::Item>);

		template<item_object T>
		inline RE::TESForm* extract_form(const T& arg)
		{
			if constexpr (std::is_pointer_v<T>)
			{
				if  constexpr (std::derived_from<std::remove_pointer_t<T>, RE::TESForm>) {
					return arg;
				}
				else if constexpr (std::same_as<std::remove_pointer_t<T>, RE::InventoryEntryData>) {
					if (arg) {
						return arg->object;
					}
				}
			}
			else if constexpr (std::same_as<T, PEPE::Item>) {
				return arg.object;
			}


			return nullptr;
		}

		template<item_object T>
		inline RE::ExtraDataList* extract_list(const T& arg)
		{
			if constexpr (std::is_pointer_v<T>)
			{
				if constexpr (std::same_as<std::remove_pointer_t<T>, RE::InventoryEntryData>) {
					if (arg && arg->extraLists && arg->extraLists->empty() != true) {
						return arg->extraLists->front();
					}
				}
			}
			else if constexpr (std::same_as<T, PEPE::Item>) {
				return arg.extraData;
			}


			return nullptr;
		}
	}
}


namespace PerkEntryPointExtenderAPI
{
	template<typename T>
	using ABIContainer = std::span<T>;

	using namespace PEPE;

	enum Version
	{
		Version1,
		Version2,
		Version3,


		Current = Version3
	};

	struct InterfaceVersion1
	{
		inline static constexpr auto VERSION = Version::Version1;

		virtual ~InterfaceVersion1() = default;

		/// <summary>
		/// Gets the current version of the interface.
		/// </summary>
		/// <returns></returns>
		virtual Version GetVersion() = 0;

		[[deprecated("This version of ApplyPerkEntryPoint doesn't include entry point flags and uses the deprecated channel feature.")]]
		virtual RequestResult ApplyPerkEntryPoint_Deprecated(RE::Actor* target, RE::PerkEntryPoint a_entryPoint, std::span<RE::TESForm*> args, void* out,
			const char* category, uint8_t channel) = 0;

	};

	struct InterfaceVersion2 : public InterfaceVersion1
	{
		inline static constexpr auto VERSION = Version::Version2;

		[[deprecated("This version of ApplyPerkEntryPoint is no longer the main point of application due to not having an extra data array.")]]
		virtual RequestResult ApplyPerkEntryPoint_Deprecated(RE::Actor* target, RE::PerkEntryPoint a_entryPoint, std::span<RE::TESForm*> args, void* out,
			const char* category, uint8_t channel, EntryPointFlag flags) = 0;

	};

	struct InterfaceVersion3 : public InterfaceVersion2
	{
		inline static constexpr auto VERSION = Version::Version3;



		virtual RequestResult ApplyPerkEntryPoint(RE::Actor* target, RE::PerkEntryPoint a_entryPoint,
			std::span<RE::TESForm*> args, std::span<RE::ExtraDataList*> extras, void* out,
			const std::string_view& category, uint8_t channel, EntryPointFlag flags) = 0;


		/// <summary>
		/// Pulls the currently loaded category being run.
		/// </summary>
		/// <returns></returns>
		virtual std::string_view GetCurrentCategory() = 0;


	};



	using CurrentInterface = InterfaceVersion3;

	inline CurrentInterface* Interface = nullptr;




	/// <summary>
	/// Accesses the Arithmetic Interface, safe to call PostLoad
	/// </summary>
	/// <param name="version"> to request.</param>
	/// <returns>Returns void* of the interface, cast to the respective version.</returns>
	inline void* RequestInterface(Version version)
	{
		typedef void* (__stdcall* RequestFunction)(Version);

		static RequestFunction request_interface = nullptr;

		HINSTANCE API = GetModuleHandle(PEPE_API_SOURCE);

		if (API == nullptr) {
#ifdef SPDLOG_H
			spdlog::critical("PerkEntryPointExtender.dll not found, API will remain non functional.");
#endif
			return nullptr;
		}

		request_interface = (RequestFunction)GetProcAddress(API, "PEPE_RequestInterfaceImpl");

		if (request_interface) {
			if (static unsigned int once = 0; once++) {
#ifdef SPDLOG_H
				spdlog::info("Successful module and request, PEPE");
#endif
			}

		}
		else {
#ifdef SPDLOG_H
			spdlog::critical("Unsuccessful module and request, PEPE");
#endif
			return nullptr;
		}

		auto intfc = (CurrentInterface*)request_interface(version);

		return intfc;
	}

	/// <summary>
	/// Accesses the ActorValueGenerator Interface, safe to call PostLoad
	/// </summary>
	/// <typeparam name="InterfaceClass">is the class derived from the interface to use.</typeparam>
	/// <returns>Casts to and returns a specific version of the interface.</returns>
	template <class InterfaceClass = CurrentInterface>
	inline  InterfaceClass* RequestInterface()
	{
		static InterfaceClass* intfc = nullptr;

		if (!intfc) {
			intfc = reinterpret_cast<InterfaceClass*>(RequestInterface(InterfaceClass::VERSION));

			if constexpr (std::is_same_v<InterfaceClass, CurrentInterface>)
				Interface = intfc;
		}

		return intfc;
	}

}


namespace PEPE
{
	inline std::string_view GetCurrentCategory()
	{
		auto* intfc = PerkEntryPointExtenderAPI::RequestInterface();

		assert(intfc);
		//No interface. Bail.
		if (!intfc)
			return inactiveCategory;

		return intfc->GetCurrentCategory();
	}
}



namespace RE
{
	inline static PEPE::RequestResult HandleEntryPointImpl(RE::PerkEntryPoint a_entryPoint, RE::Actor* a_perkOwner,
		PEPE::EntryPointFlag flags, void* out, const std::string_view& category, uint8_t channel, std::vector<RE::TESForm*> args, std::vector<RE::ExtraDataList*> extras)
	{
		auto* intfc = PerkEntryPointExtenderAPI::RequestInterface();

		assert(intfc);
		//No interface. Bail.
		if (!intfc)
			return PEPE::RequestResult::InvalidAPI;

		auto res = intfc->ApplyPerkEntryPoint(a_perkOwner, a_entryPoint, args, extras, out, category, channel, flags);

		assert(res == PEPE::RequestResult::Success);

		return res;
	}



	//Combinations
	//Stuff with no category or channel
	//stuff with no channel
	//stuff with string
	//stuff with string_view



	/// <summary>
	///
	/// </summary>
	/// <typeparam name="O"></typeparam>
	/// <typeparam name="...Args"></typeparam>
	/// <param name="a_entryPoint">The given entry point function to alias.</param>
	/// <param name="a_perkOwner">The actor who the perk runs on.</param>
	/// <param name="out">The output value of the perk entry. Set to std::nullopt if there is no out value for the function.</param>
	/// <param name="category">The category to check for the entry point, preventing anything without that group from firing.</param>
	/// <param name="...a_args">The condition targets for the entry point conditions tab of the perk.</param>
	/// <returns>Returns the result of the function, and whether the call failed due to a parameter or API issue.</returns>
	template <class O, PEPE::detail::item_object... Args>
	inline static PEPE::RequestResult HandleEntryPoint(RE::PerkEntryPoint a_entryPoint, RE::Actor* a_perkOwner, PEPE::EntryPointFlag flags, O& out,
		const std::string_view& category, uint8_t channel, Args... args)
	{


		std::vector<RE::ExtraDataList*> extras;

		//If any of these don't derive from this.
		if constexpr ((!std::derived_from<std::remove_pointer_t<Args>, RE::TESForm> || ...))
		{
			extras = { PEPE::detail::extract_list(args)... };
		}

		PEPE::BasicFormCollection<O> collector{ out };

		constexpr bool valid_collect = !std::is_empty_v<decltype(collector)>;

		void* o;

		if constexpr (valid_collect) {
			//Submit flags
			flags = (PEPE::EntryPointFlag)(flags | PEPE::EntryPointFlag::PRIVATE_UsesCollection);
			o = &collector;

		}
		else if constexpr (!std::is_same_v<std::nullopt_t, O>) {
			if  constexpr (std::is_pointer_v<O> && std::derived_from<std::remove_pointer_t<O>, RE::TESObjectREFR>) {
				o = const_cast<RE::TESObjectREFR*>(out);
			}
			else {
				o = const_cast<std::remove_const_t<O>*>(std::addressof(out));
			}
		}
		else {
			o = nullptr;
		}

		auto result = HandleEntryPointImpl(a_entryPoint, a_perkOwner, flags, o, category, channel, { PEPE::detail::extract_form(args)... }, extras);

		if constexpr (valid_collect) {
			//Submit flags
			out = collector.out;
		}

		return result;

	}

	namespace detail
	{
		//Not to be directly used, a function that has all the parameters so I don't accidentally use a different set.
		template <class O, PEPE::detail::item_object... Args>
		inline static PEPE::RequestResult HandleEntryPoint_Unpaired(RE::PerkEntryPoint a_entryPoint, RE::Actor* a_perkOwner, PEPE::EntryPointFlag flags, O& out,
			const std::string_view& category, uint8_t channel, Args... a_args)
		{
			return ::RE::HandleEntryPoint(a_entryPoint, a_perkOwner, flags, out, category, channel, a_args...);
		}


		template <class O, PEPE::detail::item_object... Args1, PEPE::detail::item_object... Args2>
		inline static PEPE::RequestResult HandleEntryPoint_Paired(RE::PerkEntryPoint a_entryPoint, RE::Actor* a_perkOwner, PEPE::EntryPointFlag flags, O& out,
			const std::string_view& category, uint8_t channel, std::pair<Args1, Args2>... a_args)
		{
			flags = (PEPE::EntryPointFlag)(flags | PEPE::EntryPointFlag::Paired);

			return ::RE::HandleEntryPoint(a_entryPoint, a_perkOwner, flags, out, category, channel, a_args.first..., a_args.second...);
		}
	}

	//With Flags/Category
	template <class O, PEPE::detail::item_object... Args>
	inline static PEPE::RequestResult HandleEntryPoint(RE::PerkEntryPoint a_entryPoint, RE::Actor* a_perkOwner, PEPE::EntryPointFlag flags, O& out, const std::string_view& category, Args... a_args)
	{
		return detail::HandleEntryPoint_Unpaired(a_entryPoint, a_perkOwner, flags, out, category, 255, a_args...);
	}

	//With Category
	template <class O, PEPE::detail::item_object... Args>
	inline static PEPE::RequestResult HandleEntryPoint(RE::PerkEntryPoint a_entryPoint, RE::Actor* a_perkOwner, O& out, const std::string_view& category, Args... a_args)
	{
		return detail::HandleEntryPoint_Unpaired(a_entryPoint, a_perkOwner, PEPE::EntryPointFlag::None, out, category, 255, a_args...);
	}


	//With Flags
	template <class O, PEPE::detail::item_object... Args>
	inline static PEPE::RequestResult HandleEntryPoint(RE::PerkEntryPoint a_entryPoint, RE::Actor* a_perkOwner, PEPE::EntryPointFlag flags, O& out, Args... a_args)
	{
		return detail::HandleEntryPoint_Unpaired(a_entryPoint, a_perkOwner, flags, out, "", 0, a_args...);
	}
	//Regular
	template <class O, PEPE::detail::item_object... Args>
	inline static PEPE::RequestResult HandleEntryPoint(RE::PerkEntryPoint a_entryPoint, RE::Actor* a_perkOwner, O& out, Args... a_args)
	{
		return detail::HandleEntryPoint_Unpaired(a_entryPoint, a_perkOwner, PEPE::EntryPointFlag::None, out, "", 0, a_args...);
	}




	//With Flags/Category, Paired
	template <class O, PEPE::detail::item_object... Args1, PEPE::detail::item_object... Args2> requires(sizeof...(Args1) > 0)
	inline static PEPE::RequestResult HandleEntryPoint(RE::PerkEntryPoint a_entryPoint, RE::Actor* a_perkOwner, PEPE::EntryPointFlag flags, O& out, const std::string_view& category, std::pair<Args1, Args2>... a_args)
	{
		return detail::HandleEntryPoint_Paired(a_entryPoint, a_perkOwner, flags, out, category, 255, a_args...);
	}

	//With Category, Paried
	template <class O, PEPE::detail::item_object... Args1, PEPE::detail::item_object... Args2> requires(sizeof...(Args1) > 0)
	inline static PEPE::RequestResult HandleEntryPoint(RE::PerkEntryPoint a_entryPoint, RE::Actor* a_perkOwner, O& out, const std::string_view& category, std::pair<Args1, Args2>... a_args)
	{
		return detail::HandleEntryPoint_Paired(a_entryPoint, a_perkOwner, PEPE::EntryPointFlag::None, out, category, 255, a_args...);
	}


	//With Flags, Paired
	template <class O, PEPE::detail::item_object... Args1, PEPE::detail::item_object... Args2> requires(sizeof...(Args1) > 0)
	inline static PEPE::RequestResult HandleEntryPoint(RE::PerkEntryPoint a_entryPoint, RE::Actor* a_perkOwner, PEPE::EntryPointFlag flags, O& out, std::pair<Args1, Args2>... a_args)
	{
		return detail::HandleEntryPoint_Paired(a_entryPoint, a_perkOwner, flags, out, "", 0, a_args...);
	}

	//Regular, Paired
	template <class O, PEPE::detail::item_object... Args1, PEPE::detail::item_object... Args2> requires(sizeof...(Args1) > 0)
	inline static PEPE::RequestResult HandleEntryPoint(RE::PerkEntryPoint a_entryPoint, RE::Actor* a_perkOwner, O& out, std::pair<Args1, Args2>... a_args)
	{
		return detail::HandleEntryPoint_Paired(a_entryPoint, a_perkOwner, PEPE::EntryPointFlag::None, out, "", 0, a_args...);
	}


}