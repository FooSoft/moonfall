/*
    Moonfall  Copyright (C) 2008  Alex Yatskov

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "Pch.h"
#include "ScriptManifest.h"
#include "Script.h"

bool ScriptManifest::LoadManifest(const char* filename)
{
    boost::shared_ptr<const Buffer> buffer = System::LoadFile(filename);
    if (!buffer)
    {
        TRACE_ERROR(boost::format("Cannot load script manifest %s") % filename);
        return false;
    }

    TiXmlDocument document;
    if (!document.Parse(static_cast<const char*>(buffer->Get())))
    {
        TRACE_ERROR(boost::format("Cannot parse script manifest %s") % filename);
        return false;
    }

    const TiXmlElement* rootElement = document.RootElement();
    if (rootElement == NULL || strcmp(rootElement->Value(), "Manifest") != 0)
    {
        return false;
    }

    for (const TiXmlElement* scriptElement = rootElement->FirstChildElement(); scriptElement != NULL; scriptElement = scriptElement->NextSiblingElement())
    {
        if (strcmp(scriptElement->Value(), "Script") != 0)
        {
            continue;
        }

        const char* asset = scriptElement->Attribute("asset");
        const char* alias = scriptElement->Attribute("alias");

        if (asset == NULL || alias == NULL)
        {
            TRACE_WARNING("Script definition is missing required fields");
            continue;
        }

        if (m_manifest.find(alias) != m_manifest.end())
        {
            TRACE_WARNING(boost::format("Duplicate script %s") % alias);
            continue;
        }

        ScriptEntry entry(asset);

        for (const TiXmlElement* parametersElement = scriptElement->FirstChildElement(); parametersElement != NULL; parametersElement = parametersElement->NextSiblingElement())
        {
            if (strcmp(parametersElement->Value(), "Parameters") != 0)
            {
                continue;
            }

            for (const TiXmlElement* parameterElement = parametersElement->FirstChildElement(); parameterElement != NULL; parameterElement = parameterElement->NextSiblingElement())
            {
                if (strcmp(parameterElement->Value(), "Parameter") != 0)
                {
                    continue;
                }

                const char* parameterName = parameterElement->Attribute("name");
                if (parameterName == NULL)
                {
                    TRACE_WARNING("Script parameter has no name");
                    continue;
                }

                ScriptParameterType parameterType = SCRIPT_PARAMETER_TYPE_STRING;
                const char* parameterTypeStr = parameterElement->Attribute("type");
                if (parameterTypeStr != NULL)
                {
                    if (strcmp(parameterTypeStr, "boolean") == 0)
                    {
                        parameterType = SCRIPT_PARAMETER_TYPE_BOOLEAN;
                    }
                    else if (strcmp(parameterTypeStr, "number") == 0)
                    {
                        parameterType = SCRIPT_PARAMETER_TYPE_NUMBER;
                    }
                    else if (strcmp(parameterTypeStr, "string") != 0)
                    {
                        TRACE_WARNING(boost::format("Unrecognized parameter type %s in script %s") % parameterTypeStr % parameterName);
                    }
                }

                const char* parameterValue = parameterElement->Attribute("value");
                if (parameterValue == NULL)
                {
                    static const char emptyString[] = "";
                    static const char emptyBoolean[] = "false";
                    static const char emptyNumber[] = "0";

                    switch (parameterType)
                    {
                    case SCRIPT_PARAMETER_TYPE_BOOLEAN:
                        parameterValue = emptyBoolean;
                        break;
                    case SCRIPT_PARAMETER_TYPE_NUMBER:
                        parameterValue = emptyNumber;
                        break;
                    default:
                        parameterValue = emptyString;
                        break;
                    }
                }

                entry.parameters.insert(std::make_pair(parameterName, ScriptParameter(parameterValue, parameterType)));
            }
        }

        m_manifest.insert(std::make_pair(alias, entry));
    }

    return true;
}

void ScriptManifest::ClearManifest()
{
    m_manifest.clear();
}

void ScriptManifest::SummarizeScripts(std::vector<ScriptSummary>* summary) const
{
    for (ScriptMap::const_iterator iter = m_manifest.begin(); iter != m_manifest.end(); ++iter)
    {
        summary->push_back(ScriptSummary(iter->first.c_str(), iter->second.filename, iter->second.parameters));
    }
}

boost::shared_ptr<const Buffer> ScriptManifest::LoadScriptCode(const char* scriptAlias) const
{
    ScriptMap::const_iterator iter = m_manifest.find(scriptAlias);
    if (iter == m_manifest.end())
    {
        TRACE_ERROR(boost::format("Cannot load script %s") % scriptAlias);
        return boost::shared_ptr<const Buffer>();
    }

    return System::LoadFile(iter->second.filename.c_str());
}
