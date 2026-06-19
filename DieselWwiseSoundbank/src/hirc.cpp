#include "hirc.h"

namespace Wwise {
	HIRC::HIRC(Reader& reader) {
		std::cout << "Reading: HIRC (Wwise objects)" << std::endl;
		section_info = Section(reader);
		reader.Read(&num_items);

		// for every item, insert its variant into "items".
		for (uint32_t i = 0; i < num_items; i++) {
			reader.PushCurrentPos();
			uint8_t item_type = reader.Read<uint8_t>();
			std::cout << "\tItem #" << i + 1 << ": ";
			reader.PopLastPos();

#ifndef NDEBUG
			if (i + 1 == 11) {
				std::cout << "";
			}
#endif

			if (VERSION != BankVersion::V2022) {
				switch ((HIRCItemTypeOld)(item_type)) {
				case HIRCItemTypeOld::Attenuation:
					std::cout << "Attenuation" << std::endl;
					items.push_back(HIRCAttenuation(reader));
					break;
				case HIRCItemTypeOld::Sound:
					std::cout << "Sound" << std::endl;
					items.push_back(HIRCSound(reader));
					break;
				case HIRCItemTypeOld::Action:
					std::cout << "Action" << std::endl;
					items.push_back(HIRCActionBase(reader));
					break;
				case HIRCItemTypeOld::Event:
					std::cout << "Event" << std::endl;
					items.push_back(HIRCEvent(reader));
					break;
				case HIRCItemTypeOld::RandomOrSequenceContainer:
					std::cout << "Random / Sequence Container" << std::endl;
					items.push_back(HIRCRandomSequenceController(reader));
					break;
				case HIRCItemTypeOld::ActorMixer:
					std::cout << "Actor Mixer" << std::endl;
					items.push_back(HIRCActorMixer(reader));
					break;
				case HIRCItemTypeOld::LayerContainer:
					std::cout << "Layer Container" << std::endl;
					items.push_back(HIRCLayerContainer(reader));
					break;
				case HIRCItemTypeOld::SwitchContainer:
					std::cout << "Switch Container" << std::endl;
					items.push_back(HIRCSwitchContainer(reader));
					break;
				case HIRCItemTypeOld::FxShareSet:
					std::cout << "FX Share Set" << std::endl;
					items.push_back(HIRCFxShareSet(reader));
					break;
				case HIRCItemTypeOld::FxCustom:
					std::cout << "FX Custom" << std::endl;
					items.push_back(HIRCFxShareSet(reader));
					break;
				case HIRCItemTypeOld::MusicTrack:
					std::cout << "Music Track" << std::endl;
					items.push_back(HIRCMusicTrack(reader));
					break;
				case HIRCItemTypeOld::MusicSegment:
					std::cout << "Music Segment" << std::endl;
					items.push_back(HIRCMusicSegment(reader));
					break;
				case HIRCItemTypeOld::MusicRandomOrSequence:
					std::cout << "Music Random / Sequence Container" << std::endl;
					items.push_back(HIRCMusicRandomSequenceController(reader));
					break;
				case HIRCItemTypeOld::MusicSwitch:
					std::cout << "Music Switch" << std::endl;
					items.push_back(HIRCMusicSwitch(reader));
					break;
				default:
					std::cout << "Unknown! will be unable to convert successfully" << std::endl;
					items.push_back(HIRCUnknown(reader));
				}
			}
			else {
				switch ((HIRCItemTypeNew)(item_type)) {
				case HIRCItemTypeNew::Attenuation:
					std::cout << "Attenuation" << std::endl;
					items.push_back(HIRCAttenuation(reader));
					break;
				case HIRCItemTypeNew::Sound:
					std::cout << "Sound" << std::endl;
					items.push_back(HIRCSound(reader));
					break;
				case HIRCItemTypeNew::Action:
					std::cout << "Action" << std::endl;
					items.push_back(HIRCActionBase(reader));
					break;
				case HIRCItemTypeNew::Event:
					std::cout << "Event" << std::endl;
					items.push_back(HIRCEvent(reader));
					break;
				case HIRCItemTypeNew::RandomOrSequenceContainer:
					std::cout << "Random / Sequence Container" << std::endl;
					items.push_back(HIRCRandomSequenceController(reader));
					break;
				case HIRCItemTypeNew::ActorMixer:
					std::cout << "Actor Mixer" << std::endl;
					items.push_back(HIRCActorMixer(reader));
					break;
				case HIRCItemTypeNew::LayerContainer:
					std::cout << "Layer Container" << std::endl;
					items.push_back(HIRCLayerContainer(reader));
					break;
				case HIRCItemTypeNew::SwitchContainer:
					std::cout << "Switch Container" << std::endl;
					items.push_back(HIRCSwitchContainer(reader));
					break;
				case HIRCItemTypeNew::FxShareSet:
					std::cout << "FX Share Set" << std::endl;
					items.push_back(HIRCFxShareSet(reader));
					break;
				case HIRCItemTypeNew::FxCustom:
					std::cout << "FX Custom" << std::endl;
					items.push_back(HIRCFxShareSet(reader));
					break;
				case HIRCItemTypeNew::MusicTrack:
					std::cout << "Music Track" << std::endl;
					items.push_back(HIRCMusicTrack(reader));
					break;
				case HIRCItemTypeNew::MusicSegment:
					std::cout << "Music Segment" << std::endl;
					items.push_back(HIRCMusicSegment(reader));
					break;
				case HIRCItemTypeNew::MusicRandomOrSequence:
					std::cout << "Music Random / Sequence Container" << std::endl;
					items.push_back(HIRCMusicRandomSequenceController(reader));
					break;
				case HIRCItemTypeNew::MusicSwitch:
					std::cout << "Music Switch" << std::endl;
					items.push_back(HIRCMusicSwitch(reader));
					break;
				default:
					std::cout << "Unknown! will be unable to convert successfully" << std::endl;
					items.push_back(HIRCUnknown(reader));
				}
			}
		}
	}

	void HIRC::Convert(Writer& writer) {
		section_info.Convert(writer);
		writer << num_items;

		// hand off conversion to individual objects

		for (uint32_t i = 0; i < num_items; i++) {
			std::cout << "#" << i + 1 << std::endl << "\t";

#ifndef NDEBUG
			if (i + 1 == 124124) {
				std::cout << "";
			}
#endif

			std::visit(overload{
				[&](HIRCAttenuation& item) {
					std::cout << "Converting Attenuation" << std::endl;
					item.Convert(writer);
				},
				[&](HIRCSound& item) {
					std::cout << "Converting Sound" << std::endl;
					item.Convert(writer);
				},
				[&](HIRCActionBase& item) {
					std::cout << "Converting Action" << std::endl;
					item.Convert(writer);
				},
				[&](HIRCEvent& item) {
					std::cout << "Converting Event" << std::endl;
					item.Convert(writer);
				},
				[&](HIRCRandomSequenceController& item) {
					std::cout << "Converting Random/Sequence Container" << std::endl;
					item.Convert(writer);
				},
				[&](HIRCActorMixer& item) {
					std::cout << "Converting Actor Mixer" << std::endl;
					item.Convert(writer);
				},
				[&](HIRCLayerContainer& item) {
					std::cout << "Converting Layer Container" << std::endl;
					item.Convert(writer);
				},
				[&](HIRCSwitchContainer& item) {
					std::cout << "Converting Switch Container" << std::endl;
					item.Convert(writer);
				},
				[&](HIRCFxShareSet& item) {
					std::cout << "Converting FX Share Set / FX Custom" << std::endl;
					item.Convert(writer);
				},
				[&](HIRCMusicTrack& item) {
					std::cout << "Converting Music Track" << std::endl;
					item.Convert(writer);
				},
				[&](HIRCMusicSegment& item) {
					std::cout << "Converting Music Segment" << std::endl;
					item.Convert(writer);
				},
				[&](HIRCMusicRandomSequenceController& item) {
					std::cout << "Converting Music Random / Sequence Container" << std::endl;
					item.Convert(writer);
				},
				[&](HIRCMusicSwitch& item) {
					std::cout << "Converting Music Switch" << std::endl;
					item.Convert(writer);
				},
				[&](HIRCUnknown& item) {
					throw std::runtime_error("ERROR: Unable to convert unknown item type!");
				},
				[&](auto& item)
				{
					std::cout << "Unhandled type" << std::endl;
				}
			}, items[i]);
		}

		section_info.UpdateSize(writer);
	}
}