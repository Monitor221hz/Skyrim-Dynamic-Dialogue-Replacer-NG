#pragma once


#define PI 3.1415926535897932f
#define TWOTHIRDS_PI 2.0943951023931955f
#define TWO_PI 6.2831853071795865f
#define PI2 1.5707963267948966f
#define PI3 1.0471975511965977f
#define PI4 0.7853981633974483f
#define PI8 0.3926990816987242f

using namespace RE;

namespace PointerUtil //yoinked po3's code
{
template <class T, class U>
inline auto adjust_pointer(U* a_ptr, std::ptrdiff_t a_adjust) noexcept
{
	auto addr = a_ptr ? reinterpret_cast<std::uintptr_t>(a_ptr) + a_adjust : 0;
	if constexpr (std::is_const_v<U> && std::is_volatile_v<U>) {
		return reinterpret_cast<std::add_cv_t<T>*>(addr);
	} else if constexpr (std::is_const_v<U>) {
		return reinterpret_cast<std::add_const_t<T>*>(addr);
	} else if constexpr (std::is_volatile_v<U>) {
		return reinterpret_cast<std::add_volatile_t<T>*>(addr);
	} else {
		return reinterpret_cast<T*>(addr);
	}
}
}
namespace SystemUtil
{
    struct File 
    {
        
	static std::vector<std::string> GetConfigs(std::string_view a_folder, std::string_view a_suffix, std::string_view a_extension = ".ini"sv)
	{
		std::vector<std::string> configs{};

		for (const auto iterator = std::filesystem::directory_iterator(a_folder); const auto& entry : iterator) {
			if (entry.exists()) {
				if (const auto& path = entry.path(); !path.empty() && path.extension() == a_extension) {
					if (const auto& fileName = entry.path().string(); fileName.rfind(a_suffix) != std::string::npos) {
						configs.push_back(fileName);
					}
				}
			}
		}

		std::ranges::sort(configs);

		return configs;
	}
    };
}

namespace KeyUtil 
{

    enum class MACRO_LIMITS {
        kMaxMacros = 282
    };

    enum class KBM_OFFSETS {
		// first 256 for keyboard, then 8 mouse buttons, then mouse wheel up, wheel down, then 16 gamepad buttons
		kMacro_KeyboardOffset = 0,		// not actually used, just for self-documentation
		kMacro_NumKeyboardKeys = 256,

		kMacro_MouseButtonOffset = kMacro_NumKeyboardKeys,	// 256
		kMacro_NumMouseButtons = 8,

		kMacro_MouseWheelOffset = kMacro_MouseButtonOffset + kMacro_NumMouseButtons,	// 264
		kMacro_MouseWheelDirections = 2,

		kMacro_GamepadOffset = kMacro_MouseWheelOffset + kMacro_MouseWheelDirections,	// 266
		kMacro_NumGamepadButtons = 16,

			// 282
	};

	enum class GAMEPAD_OFFSETS {
		kGamepadButtonOffset_DPAD_UP = static_cast<int>(KBM_OFFSETS::kMacro_GamepadOffset),	// 266
		kGamepadButtonOffset_DPAD_DOWN,
		kGamepadButtonOffset_DPAD_LEFT,
		kGamepadButtonOffset_DPAD_RIGHT,
		kGamepadButtonOffset_START,
		kGamepadButtonOffset_BACK,
		kGamepadButtonOffset_LEFT_THUMB,
		kGamepadButtonOffset_RIGHT_THUMB,
		kGamepadButtonOffset_LEFT_SHOULDER,
		kGamepadButtonOffset_RIGHT_SHOULDER,
		kGamepadButtonOffset_A,
		kGamepadButtonOffset_B,
		kGamepadButtonOffset_X,
		kGamepadButtonOffset_Y,
		kGamepadButtonOffset_LT,
		kGamepadButtonOffset_RT	// 281
	};


    struct Interpreter
    {
        public: 
        static uint32_t GamepadMaskToKeycode(uint32_t keyMask) {
	switch (keyMask) {
		case 0x001:		return static_cast<int>(GAMEPAD_OFFSETS::kGamepadButtonOffset_DPAD_UP);
		case 0x002:		return static_cast<int>(GAMEPAD_OFFSETS::kGamepadButtonOffset_DPAD_DOWN);
		case 0x004:		return static_cast<int>(GAMEPAD_OFFSETS::kGamepadButtonOffset_DPAD_LEFT);
		case 0x008:		return static_cast<int>(GAMEPAD_OFFSETS::kGamepadButtonOffset_DPAD_RIGHT);
		case 0x0010:			return static_cast<int>(GAMEPAD_OFFSETS::kGamepadButtonOffset_START);
		case 0x0020:			return static_cast<int>(GAMEPAD_OFFSETS::kGamepadButtonOffset_BACK);
		case 0x0040:		return static_cast<int>(GAMEPAD_OFFSETS::kGamepadButtonOffset_LEFT_THUMB);
		case 0x0080:	return static_cast<int>(GAMEPAD_OFFSETS::kGamepadButtonOffset_RIGHT_THUMB);
		case 0x0100:	return static_cast<int>(GAMEPAD_OFFSETS::kGamepadButtonOffset_LEFT_SHOULDER);
		case 0x0200: return static_cast<int>(GAMEPAD_OFFSETS::kGamepadButtonOffset_RIGHT_SHOULDER);
		case 0x1000:				return static_cast<int>(GAMEPAD_OFFSETS::kGamepadButtonOffset_A);
		case 0x2000:				return static_cast<int>(GAMEPAD_OFFSETS::kGamepadButtonOffset_B);
		case 0x4000:				return static_cast<int>(GAMEPAD_OFFSETS::kGamepadButtonOffset_X);
		case 0x8000:				return static_cast<int>(GAMEPAD_OFFSETS::kGamepadButtonOffset_Y);
		case 0x9:							return static_cast<int>(GAMEPAD_OFFSETS::kGamepadButtonOffset_LT);
		case 0xA:							return static_cast<int>(GAMEPAD_OFFSETS::kGamepadButtonOffset_RT);
		default:							return 282; // Invalid
	}
}

    };

}

namespace Util
{
    struct String
    {
		static std::vector<std::string> Split(const std::string& a_str, std::string_view a_delimiter)
		{
			auto range = a_str | std::ranges::views::split(a_delimiter) | std::ranges::views::transform([](auto&& r) { return std::string_view(r); });
			return { range.begin(), range.end() };
		}



        static bool iContains(std::string_view a_str1, std::string_view a_str2)
		{
			if (a_str2.length() > a_str1.length()) {
				return false;
			}

			const auto subrange = std::ranges::search(a_str1, a_str2, [](unsigned char ch1, unsigned char ch2) {
				return std::toupper(ch1) == std::toupper(ch2);
			});

			return !subrange.empty();
		}

		static bool iEquals(std::string_view a_str1, std::string_view a_str2)
		{
			return std::ranges::equal(a_str1, a_str2, [](unsigned char ch1, unsigned char ch2) {
				return std::toupper(ch1) == std::toupper(ch2);
			});
		}

		// https://stackoverflow.com/a/35452044
		static std::string Join(const std::vector<std::string>& a_vec, std::string_view a_delimiter)
		{
			return std::accumulate(a_vec.begin(), a_vec.end(), std::string{},
				[a_delimiter](const auto& str1, const auto& str2) {
					return str1.empty() ? str2 : str1 + a_delimiter.data() + str2;
				});
		}

        static std::vector<float> ToFloatVector(const std::vector<std::string> stringVector)
        {
            std::vector<float> floatNumbers; 
            for(auto str : stringVector)
            {
                float num = atof(str.c_str());
                floatNumbers.push_back(num);
            }
            return floatNumbers;
        }
        static std::string ToLower(std::string_view a_str)
		{
			std::string result(a_str);
			std::ranges::transform(result, result.begin(), [](unsigned char ch) { return static_cast<unsigned char>(std::tolower(ch)); });
			return result;
		}

		static std::string ToUpper(std::string_view a_str)
		{
			std::string result(a_str);
			std::ranges::transform(result, result.begin(), [](unsigned char ch) { return static_cast<unsigned char>(std::toupper(ch)); });
			return result;
		}


    };

}

namespace MathUtil
{
    struct Angle 
    {
        [[nodiscard]] constexpr static float DegreeToRadian(float a_angle)
        {
            return a_angle * 0.017453292f;
        }

        [[nodiscard]] constexpr static float RadianToDegree(float a_radian)
        {
            return a_radian * 57.295779513f;
        }

        static NiPoint3 ToRadianVector(float x, float y, float z)
        {
            RE::NiPoint3 rotationVector{ 0.f, 0.f, 0.f };

            rotationVector.x = DegreeToRadian(x); 
            rotationVector.y = DegreeToRadian(y); 
            rotationVector.z = DegreeToRadian(z); 
            return rotationVector; 
        }

        static float NormalAbsoluteAngle(float a_angle)
        {
            while (a_angle < 0)
                a_angle += TWO_PI;
            while (a_angle > TWO_PI)
                a_angle -= TWO_PI;
            return a_angle;

            // return fmod(a_angle, TWO_PI) >= 0 ? a_angle : (a_angle + TWO_PI);
        }

        static float NormalRelativeAngle(float a_angle)
        {
            while (a_angle > PI)
                a_angle -= TWO_PI;
            while (a_angle < -PI)
                a_angle += TWO_PI;
            return a_angle;

            // return fmod(a_angle, TWO_PI) >= 0 ? (a_angle < PI) ? a_angle : a_angle - TWO_PI : (a_angle >= -PI) ? a_angle : a_angle + TWO_PI;
        }
    }; 
}
namespace ObjectUtil
{
    struct Transform
    {
        static void TranslateTo(RE::BSScript::IVirtualMachine *vm, RE::VMStackID stackID, RE::TESObjectREFR *object, float afX, float afY, float afZ, float afAngleX, float afAngleY, float afAngleZ, float afSpeed, float afMaxRotationSpeed)
        {
            using func_t = decltype(TranslateTo);
            REL::Relocation<func_t> func{RELOCATION_ID(55706, 56237)};
            func(vm, stackID, object, afX, afY, afZ, afAngleX, afAngleY, afAngleZ, afSpeed, afMaxRotationSpeed);
        }

        static float InterpAngleTo(float a_current, float a_target, float a_deltaTime, float a_interpSpeed)
        {
            if (a_interpSpeed <= 0.f)
            {
                return a_target;
            }

            const float distance = MathUtil::Angle::NormalRelativeAngle(a_target - a_current);

            if (distance * distance < FLT_EPSILON)
            {
                return a_target;
            }

            const float delta = distance * Clamp(a_deltaTime * a_interpSpeed, 0.f, 1.f);

            return a_current + delta;
        }

        static float Clamp(float value, float min, float max)
        {
            return value < min ? min : value < max ? value
                                                   : max;
        }
    };
}


namespace AnimUtil
{
    struct Idle
    {
            static bool Play(RE::TESIdleForm* idle, RE::Actor* actor, RE::DEFAULT_OBJECT action, RE::Actor* target)
            {
            if (actor && actor->GetActorRuntimeData().currentProcess)
            {
                typedef bool (*func_t)(RE::AIProcess *, RE::Actor *, RE::DEFAULT_OBJECT, RE::TESIdleForm *, bool, bool, RE::Actor *);
                REL::Relocation<func_t> func{RELOCATION_ID(38290, 39256)};
                return func(actor->GetActorRuntimeData().currentProcess, actor, action, idle, true, true, target);
            }
            return false;
            }
    };
}

namespace FormUtil
{
    struct Form
    {
            static RE::TESForm *GetFormFromMod(std::string modname, uint32_t formid)
            {
            if (!modname.length() || !formid)
                return nullptr;
            RE::TESDataHandler *dh = RE::TESDataHandler::GetSingleton();
            return dh->LookupForm(formid, modname); 
            // RE::TESFile *modFile = nullptr;
            // for (auto it = dh->files.begin(); it != dh->files.end(); ++it)
            // {
            //     RE::TESFile *f = *it;
            //     if (strcmp(f->fileName, modname.c_str()) == 0)
            //     {
            //         modFile = f;
            //         break;
            //     }
            // }
            // if (!modFile)
            //     return nullptr;
            // uint8_t modIndex = modFile->compileIndex;
            // uint32_t id = formid;
            // if (modIndex < 0xFE)
            // {
            //     id |= ((uint32_t)modIndex) << 24;
            // }
            // else
            // {
            //     uint16_t lightModIndex = modFile->smallFileCompileIndex;
            //     if (lightModIndex != 0xFFFF)
            //     {
            //         id |= 0xFE000000 | (uint32_t(lightModIndex) << 12);
            //     }
            // }
            // return RE::TESForm::LookupByID(id);
            }

            static RE::TESForm *GetFormFromMod(std::string modname, std::string formIDString)
            {
                if (formIDString.length() == 0) return nullptr; 

                uint32_t formID = std::stoi(formIDString, 0, 16); 
                return GetFormFromMod(modname,formID); 
            } 

            static RE::TESForm *GetFormFromConfigString(std::string str, std::string_view delimiter)
            {
                std::vector<std::string> splitData = Util::String::Split(str, delimiter); 
                if (splitData.size() < 2) return nullptr;  
                return GetFormFromMod(splitData[1], splitData[0]);
            }
            static RE::TESForm *GetFormFromConfigString(std::string str)
            {
                return GetFormFromConfigString(str, "~"sv); 
            }

            static RE::FormID GetFormIDFromMod(std::string relativeFormIDString, std::string modName)
            {
                if (relativeFormIDString.length() == 0) return 0x0; 


                uint32_t relativeFormID = std::stoi(relativeFormIDString,  0, 16); 
                auto* dataHandler = TESDataHandler::GetSingleton(); 

                if (!dataHandler) return 0x0; 

                return dataHandler->LookupFormID(relativeFormID, modName); 
            }

            static RE::FormID GetFormIDFromConfigString(std::string str, std::string_view delimiter)
            {
                std::vector<std::string> splitData = Util::String::Split(str, delimiter); 
                if (splitData.size() < 2) return 0x0; 
                return GetFormIDFromMod(splitData[0], splitData[1]);
            }
            static RE::FormID GetFormIDFromConfigString(std::string str)
            {
                return GetFormIDFromConfigString(str, "~"sv); 
            }


    };

    struct Quest 
    {
        public:
            static BGSBaseAlias *FindAliasByName(std::string_view name, TESQuest *owningQuest)
            {
                RE::BSWriteLockGuard AliasLock{owningQuest->aliasAccessLock};
                for (auto *alias : owningQuest->aliases)
                {
                std::string aliasName = alias->aliasName.c_str();
                if (aliasName == name)
                    return alias;
                }
                return nullptr;
            }
    };
}
namespace NifUtil
{
    struct Node
		{
            static NiAVObject* Clone(NiAVObject* original)
            {
                typedef NiAVObject* (*func_t)(NiAVObject* avObj);
		        REL::Relocation<func_t> func{ RELOCATION_ID(68835, 70187) };
		        return func(original);
            }
			static RE::NiAVObject* GetNiObject(
				RE::NiNode*              a_root,
				const RE::BSFixedString& a_name)
			{
				return a_root->GetObjectByName(a_name);
			}

			static void AttachToNode(
				RE::NiAVObject* a_object,
				RE::NiNode*     a_node)
			{
				if (a_object->parent != a_node)
				{
					a_node->AttachChild(a_object, true);
				}
			}

            static std::vector<BSGeometry*> GetAllGeometries(RE::NiAVObject* root)
            {
                std::vector<BSGeometry*> geometries; 
                RE::BSVisit::TraverseScenegraphGeometries(root, [&](BSGeometry* geom)-> RE::BSVisit::BSVisitControl 
                {
                    geometries.emplace_back(geom); 
                    return RE::BSVisit::BSVisitControl::kContinue;
                }
				); 
                return geometries;
            }

		};
    struct Armature
    {
        static RE::NiNode* GetActorNode(RE::Actor* actor, std::string nodeName)
        {
                auto root = actor->Get3D();
                if (!root) return nullptr;

                auto bone = root->GetObjectByName(nodeName);
                if (!bone) return nullptr;

                auto node = bone->AsNode();
                if (!node) return nullptr;

                return node;
        }

        static void AttachToNode(RE::NiAVObject* obj, RE::Actor* actor, std::string nodeName)
        {
            auto* node = GetActorNode(actor, nodeName);
            if (node)
            {
                node->AttachChild(obj, true);
                SKSE::log::info("Object Attached");
            }
        }
    };
    struct Collision
    {
        static bool ToggleMeshCollision(RE::NiAVObject* root,RE::bhkWorld* world, bool collisionState)
        {
            constexpr auto no_collision_flag = static_cast<std::uint32_t>(RE::CFilter::Flag::kNoCollision);
					if (root && world) {
						
							RE::BSWriteLockGuard locker(world->worldLock);

							RE::BSVisit::TraverseScenegraphCollision(root, [&](RE::bhkNiCollisionObject* a_col) -> RE::BSVisit::BSVisitControl {
								if (auto hkpBody = a_col->body ? static_cast<RE::hkpWorldObject*>(a_col->body->referencedObject.get()) : nullptr; hkpBody) {
									auto& filter = hkpBody->collidable.broadPhaseHandle.collisionFilterInfo;
									if (!collisionState) {
										filter |= no_collision_flag;
									} else {
										filter &= ~no_collision_flag;
									}
								}
								return RE::BSVisit::BSVisitControl::kContinue;
							});
					}
                    else 
                    {
                        return false;
                    }
            return true;
        }
         static bool RemoveMeshCollision(RE::NiAVObject* root,RE::bhkWorld* world, bool collisionState)
        {
            constexpr auto no_collision_flag = static_cast<std::uint32_t>(RE::CFilter::Flag::kNoCollision);
					if (root && world) {
						
							RE::BSWriteLockGuard locker(world->worldLock);

							RE::BSVisit::TraverseScenegraphCollision(root, [&](RE::bhkNiCollisionObject* a_col) -> RE::BSVisit::BSVisitControl {
								if (auto hkpBody = a_col->body ? static_cast<RE::hkpWorldObject*>(a_col->body->referencedObject.get()) : nullptr; hkpBody) {
									auto& filter = hkpBody->collidable.broadPhaseHandle.collisionFilterInfo;
									if (!collisionState) {
										filter |= no_collision_flag;
									} else {
										filter &= ~no_collision_flag;
									}
								}
								return RE::BSVisit::BSVisitControl::kContinue;
							});
					}
                    else 
                    {
                        return false;
                    }
            return true;
        }
    };

   
}

