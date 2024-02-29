// Copyright Epic Games, Inc. All Rights Reserved.

#include "HealthComponent.h"

UHealthComponent::UHealthComponent() {
}


void UHealthComponent::InitHealth(float maxHP) {
	SetMaxHealth(maxHP);
	SetHealth(maxHP);
}

void UHealthComponent::SetHealth(float HP) {
	health = HP;
}
float UHealthComponent::GetHealth() {
	return health;
}

void UHealthComponent::SetMaxHealth(float maxHP) {
	maxHealth = maxHP;
}
float UHealthComponent::GetMaxHealth() {
	return maxHealth;
}

float UHealthComponent::GetHealthPercentage() {
	return health / maxHealth;
}

void UHealthComponent::SetDied(bool HPState) {
	died = HPState;
}
bool UHealthComponent::GetDied() {
	return died;
}

void UHealthComponent::UpdateHP(float modifier) {
	float deltaHealth = health + modifier;

	// if player is died
	if (deltaHealth <= 0.0f) {
		health = 0.0f;
		SetDied(true);
	}
	// if the healing exceed -> is able to create more stuff
	else if (deltaHealth > maxHealth) {
		health = maxHealth;
		ExceedHealth = ExceedHealth + (deltaHealth - maxHealth);
	}
	else {
		health = health + modifier;
	}
}

// min -> -1, max -> +1
void UHealthComponent::UpdateByCurrentPercentage(float percentage) {
	float healthToModify = health * percentage;
	UpdateHP(-healthToModify);
}

// min -> -1, max -> +1
void UHealthComponent::UpdateByMaxHealthPercentage(float percentage) {
	float healthToModify = maxHealth * percentage;
	UpdateHP(-healthToModify);
}

// Difference by health -> there never going to have a max health modifier that depend on current health
void UHealthComponent::UpdateMaxHealth(float modifier) {
	float deltaHP = maxHealth + modifier;
	if (deltaHP < MaxInt) {
		maxHealth = deltaHP;
	}
	else {
		maxHealth = MaxInt;
	}
}

void UHealthComponent::UpdateMaxHealthByPercentage(float percentage) {
	float deltaHP = maxHealth + maxHealth * percentage;
	if (deltaHP < MaxInt) {
		maxHealth = deltaHP;
	}
	else {
		maxHealth = MaxInt;
	}
}

void UHealthComponent::RefillMaxHealth() {
	health = maxHealth;
}