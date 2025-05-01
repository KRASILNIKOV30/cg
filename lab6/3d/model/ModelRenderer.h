#pragma once

class Model;

class ModelRenderer
{
public:
	ModelRenderer() = default;

	ModelRenderer(const ModelRenderer& other) = delete;
	ModelRenderer& operator=(const ModelRenderer& other) = delete;

	static void RenderModel(Model const& model);
};