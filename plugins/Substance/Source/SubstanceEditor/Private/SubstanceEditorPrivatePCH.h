// Copyright 2016 Allegorithmic Inc. All rights reserved.
// File: SubstanceEditorPrivatePCH.h

#pragma once

//NOTE:: This is added to prevent including framework classes outside of our module. The allows C++ projects
//to use our core classes without breaking encapsulation for our framework headers.
#define SUBSTANCE_FRAMEWORK_INCLUDED 1

#include "UnrealEd.h"
#include "Factories.h"

#include "EdGraphSchema_K2.h"
#include "K2Node.h"
#include "KismetCompiler.h"

#include "ISubstanceEditor.h"
#include "SubstanceEditorClasses.h"
#include "SSubstanceEditorPanel.h"

#include "SubstanceCoreClasses.h"
#include "SubstanceImageInputFactory.h"
#include "ReimportSubstanceImageInputFactory.h"
#include "ReimportSubstanceFactory.h"

DEFINE_LOG_CATEGORY_STATIC(LogSubstanceEditor, Log, All);
