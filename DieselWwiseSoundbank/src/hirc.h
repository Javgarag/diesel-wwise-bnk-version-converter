#pragma once
#include "reader.h"
#include "bkhd.h"

#include "HIRC/attenuation.h"
#include "HIRC/action.h"
#include "HIRC/actor_mixer.h"
#include "HIRC/event.h"
#include "HIRC/layer_container.h"
#include "HIRC/random_sequence_controller.h"
#include "HIRC/sound.h"
#include "HIRC/switch_container.h"
#include "HIRC/fx_share_set.h"
#include "HIRC/music_track.h"
#include "HIRC/music_segment.h"
#include "HIRC/music_switch.h"
#include "HIRC/music_random_sequence_controller.h"
#include "HIRC/bus.h"
#include "HIRC/audio_device.h"

#include <cstdint>
#include <vector>

namespace Wwise {
	struct HIRC {
		Section section_info;

		uint32_t num_items;
		long long num_items_pos = -1;

		std::vector<std::variant<
			HIRCUnknown, 
			HIRCAttenuation, 
			HIRCEvent, 
			HIRCSound, 
			HIRCActionBase, 
			HIRCRandomSequenceController, 
			HIRCActorMixer, 
			HIRCLayerContainer, 
			HIRCSwitchContainer, 
			HIRCFxShareSet, 
			HIRCMusicTrack, 
			HIRCMusicSegment, 
			HIRCMusicRandomSequenceController,
			HIRCMusicSwitch,
			HIRCBus,
			HIRCAudioDevice>> items;

		HIRC() = default;
		HIRC(Reader& reader);
		void Convert(Writer& writer);
		void UpdateItemCount(uint32_t new_count, Writer& writer);
	};
}