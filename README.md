# The Problem

Previously, voice file paths for NPC dialogue were tied the voice type, plugin name, quest name, response name, and formID, and the paths were determined at runtime by parsing this data together. This meant modders could not define their own voice file paths but instead use the ones generated for them based on this data.

There was no easy way to reuse, replace, or add dialogue files without having to copy over, move, and rename these files. This made both modification and creation of dialogue very time consuming and mentally draining to do.
# The Solution

DDR provides a flexible alternate method via json file to set voice file paths for any single line of dialogue separately, as well as allowing modders to define custom paths. In addition to this, the framework allows for pluginless edits to subtitle texts.

The framework allows users to have as much control as desired for what should be replaced; a single replacement for multiple voices in a response, one replacement per voice type in a response, or even replacement for a single part of one response.

It’s all done in a json file format that is readable, easy to understand, and modifiable for the end user.

In the future, DDR will be able to replace dialogue based on conditions, surpassing a tedious part of CK development.

## Disclaimer

DDR is still in development and additional features such as conditions are yet to come.

## Documentation (for mod authors)

An example of a valid json document that DDR can read, placed in …\Data\Sound\Voice\DynamicDialogueReplacer

Things to note;

`Group01` – name of the group container. A wrapper object is required for any replacement object, to keep things organized. Imagine multiple quests and even more dialogue lines, this forces modders to stay neat and readable to users.

`topicInfo` – the formID and originating plugin.

`voiceTypes` – A list of the editorIDs of the voice types that the replacement should include.

`responseIndex` – The index of the response form to replace. Usually, speeches or monologues are each broken into multiple files after which they are added alongside each other in CK.

`overridePath` – The path to the voice file that should override the path assigned by the vanilla game. Can be formatted at runtime by putting $ in the beginning, then using the appropriate tag.

`overrideSubtitle` – The subtitle text that should override the text defined in the response.

 
```json
{
    "responseMap":
    {
        "Group01": 
        {
            "MikaelSing": 
            {
                "topicInfo": "0x5901~Example.esp",
                "voiceTypes":["MaleYoungEager"],
                "responseIndex": 1, 
                "overridePath": "$Data\\Sound\\Voice\\Skyrim.esm\\[VOICE_TYPE]\\Favor013_Favor013PersuadeS_000CD119_1.wav",
                "overrideSubtitle":"Never gonna give you up. Never gonna let you down. Never gonna run around and desert you."
            },
            "GuardSing":
            {
                "topicInfo":"0x5904~Example.esp",
                "voiceTypes":["MaleGuard", "MaleNordCommander", "MaleSoldier", "FemaleNord", "MaleNord", "MaleCommander", "MaleYoungEager"],
                "responseIndex":1,
                "overridePath":"$Data\\Sound\\Voice\\[TOPIC_MOD_FILE]\\[VOICE_TYPE]\\DialogueCr_DGCrimePersuade_00109BAC_1.wav",
                "overrideSubtitle":"Never gonna make you cry, Never gonna say goodbye, Never gonna tell a lie and hurt you."
            },
            "MulushAcceptPersuade": 
            {
                "topicInfo": "0x00069696~Example.esl",
                "voiceTypes":["MaleOrc"],
                "responseIndex": 1, 
                "overridePath": "$Data\\Sound\\Voice\\[VOICE_MOD_FILE]\\MaleOrc\\Favor013_Favor013PersuadeB_000554D9_1.wav",
                "_comment":"TIF__000CD114"
            },
            "IrnskarrTalk":
            {
                "topicInfo": "0x1EE04~Example.esp",
                "voiceTypes":["Keanu"],
                "responseIndex": 1, 
                "overrideSubtitle":"Yeah, I'm thinking I'm back.",
                "overridePath": "Data\\Test\\wick_01.wav"
            }
        }
    }
}
```
