// Copyright Epic Games, Inc. All Rights Reserved.

#include "PowerComponent.h"

UPowerComponent::UPowerComponent() {
}

// Setter and Getter of basic values
void UPowerComponent::SetPower(float PowerValue) {
	Power = PowerValue;
}

float UPowerComponent::GetPower() {
	return Power;
}
void UPowerComponent::SetCritChance(float critChanceValue) {
	CritChance = critChanceValue;
}
float UPowerComponent::GetCritChance() {
	return CritChance;
}
void UPowerComponent::SetCritMultiplier(float critMultiplierValue) {
	CritMultipier = critMultiplierValue;
}
float UPowerComponent::GetCritMultiplier() {
	return CritMultipier;
}

// Updates Values
void UPowerComponent::UpdateFlatPower(float modifier) {
	Power = Power + modifier;
}
// this percentage -> min = -1, max = 1
void UPowerComponent::UpdatePercentagePower(float modifier) {
	float PowerPercentage = Power * modifier;
	UpdateFlatPower(Power);
}
void UPowerComponent::UpdateCritChance(float modifier) {
	CritChance = CritChance + CritChance * modifier;
}
void UPowerComponent::UpdateCritMultiplier(float modifier) {
	CritMultipier = CritMultipier + CritMultipier * modifier;
}