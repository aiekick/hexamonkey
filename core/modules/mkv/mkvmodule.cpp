//This file is part of the HexaMonkey project, a multimedia analyser
//Copyright (C) 2013  Sevan Drapeau-Martin, Nicolas Fleury

//This program is free software; you can redistribute it and/or
//modify it under the terms of the GNU General Public License
//as published by the Free Software Foundation; either version 2
//of the License, or (at your option) any later version.

//This program is distributed in the hope that it will be useful,
//but WITHOUT ANY WARRANTY; without even the implied warranty of
//MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//GNU General Public License for more details.

//You should have received a copy of the GNU General Public License
//along with this program; if not, write to the Free Software
//Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.

#include "mkvmodule.h"

#include <fstream>
#include <streambuf>
#include <map>

#include "rapidxml.hpp"
#include "bitutil.h"
#include "strutil.h"
#include "fileutil.h"

#include "standardtypes.h"
#include "ebmltypes.h"




using namespace rapidxml;
using namespace ebmlTypes;

void MkvModule::addFormatDetection(StandardFormatDetector::Adder &formatAdder)
{
    formatAdder.addMagicNumber("1a 45 df a3 xx xx xx xx xx xx xx xx xx xx xx xx xx xx xx xx xx xx xx xx 6d 61 74 72 6f 73 6b 61");
    formatAdder.addExtension("mkv");
}

void MkvModule::requestImportations(std::vector<std::string> &formatRequested)
{
    formatRequested.push_back("ebml");
}

bool MkvModule::doLoad()
{
    std::string path = "mkvmodel.xml";
    if(!fileExists(path))
    {
        path = "../core/modules/mkv/mkvmodel.xml";
    }
    else if(!fileExists(path))
    {
        std::cerr<<"MKV model file not found"<<std::endl;
        return false;
    }
    std::ifstream modelFile(path);

    std::string mkvModelText((std::istreambuf_iterator<char>(modelFile)),
                     std::istreambuf_iterator<char>());

    xml_document<> mkvModelParser;
    mkvModelParser.parse<0>(&mkvModelText[0]);

    for(xml_node<>* nodeFrame = mkvModelParser.first_node()->first_node();
        nodeFrame;
        nodeFrame = nodeFrame->next_sibling())
    {
        int64_t id = parseId(nodeFrame->first_attribute("id")->value());
        std::string name = nodeFrame->first_attribute("name")->value();
        std::string type = nodeFrame->first_attribute("type")->value();

        addParser(name);
        const ObjectTypeTemplate& typeTemplate = newTemplate(name);
        setSpecification(EBMLElement(id), typeTemplate());

        for(int i =0;i<numberOfTypeElements;++i)
        {
            if(type == typeElementAtributes[i])
            {
                setExtension(typeTemplate, getTemplate(typeElements[i])());
                break;
            }
        }
    }
    return true;
}


int64_t MkvModule::parseId(char* str)
{
    int number = fromHex(*(str+2));
    int64_t value = number & ~(1<<msb(number));
    for(char* ch = str+3; *ch; ++ch)
    {
        value = value<<4 | fromHex(*ch);
    }
    return value;
}