// Copyright (c) Microsoft Corporation.
// Licensed under the MIT license.
// - A couple helper functions for serializing/deserializing a KeyMapping
//   to/from json.
//
// Author(s):
// - Mike Griese - May 2019

#include "pch.h"
#include "ActionMap.h"
#include "ActionAndArgs.h"
#include "KeyChordSerialization.h"
#include "JsonUtils.h"

#include "Command.h"

using namespace winrt::Microsoft::Terminal::Control;
using namespace winrt::Microsoft::Terminal::Settings::Model;

namespace winrt::Microsoft::Terminal::Settings::Model::implementation
{
    // Method Description:
    // - Deserialize an ActionMap from the array `json`. The json array should contain
    //   an array of serialized `Command` objects.
    // - These actions are added to the `ActionMap`, where we automatically handle
    //   overwriting and unbinding actions.
    // Arguments:
    // - json: an array of Json::Value's to deserialize into our ActionMap.
    // Return value:
    // - a list of warnings encountered while deserializing the json
    std::vector<SettingsLoadWarnings> ActionMap::LayerJson(const Json::Value& json)
    {
        // It's possible that the user provided keybindings have some warnings in
        // them - problems that we should alert the user to, but we can recover
        // from. Most of these warnings cannot be detected later in the Validate
        // settings phase, so we'll collect them now.
        std::vector<SettingsLoadWarnings> warnings;

        for (const auto& cmdJson : json)
        {
            if (!cmdJson.isObject())
            {
                continue;
            }

            AddAction(*Command::FromJson(cmdJson, warnings));
        }

        return warnings;
    }

    // Method Description:
    // - Takes the KeyModifier flags from Terminal and maps them to the WinRT types which are used by XAML
    // Return Value:
    // - a Windows::System::VirtualKeyModifiers object with the flags of which modifiers used.
    Windows::System::VirtualKeyModifiers ActionMap::ConvertVKModifiers(KeyModifiers modifiers)
    {
        Windows::System::VirtualKeyModifiers keyModifiers = Windows::System::VirtualKeyModifiers::None;

        if (WI_IsFlagSet(modifiers, KeyModifiers::Ctrl))
        {
            keyModifiers |= Windows::System::VirtualKeyModifiers::Control;
        }
        if (WI_IsFlagSet(modifiers, KeyModifiers::Shift))
        {
            keyModifiers |= Windows::System::VirtualKeyModifiers::Shift;
        }
        if (WI_IsFlagSet(modifiers, KeyModifiers::Alt))
        {
            // note: Menu is the Alt VK_MENU
            keyModifiers |= Windows::System::VirtualKeyModifiers::Menu;
        }
        if (WI_IsFlagSet(modifiers, KeyModifiers::Windows))
        {
            keyModifiers |= Windows::System::VirtualKeyModifiers::Windows;
        }

        return keyModifiers;
    }
}
