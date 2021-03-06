#pragma once

#ifdef IOS_REF
#undef  IOS_REF
#define IOS_REF (*(pSdkManager->GetIOSettings()))
#endif

using namespace System;
using namespace System::Collections::Generic;
using namespace System::IO;
using namespace System::Runtime::InteropServices;
using namespace SharpDX;

#define WITH_MARSHALLED_STRING(name,str,block)\
	{ \
		char* name; \
		try \
		{ \
			name = StringToCharArray(str); \
			block \
		} \
		finally \
		{ \
			Marshal::FreeHGlobal((IntPtr)name); \
		} \
	}

namespace AssetStudio {

	public ref class Fbx
	{
	public:
		static Vector3 QuaternionToEuler(Quaternion q);
		static Quaternion EulerToQuaternion(Vector3 v);

		ref class Exporter
		{
		public:
			static void Export(String^ path, IImported^ imported, bool EulerFilter, float filterPrecision, bool allFrames, bool allBones, bool skins, float boneSize, bool flatInbetween, bool compatibility);
			static void ExportMorph(String^ path, IImported^ imported, bool morphMask, bool flatInbetween, bool skins, float boneSize, bool compatibility);

		private:
			HashSet<String^>^ frameNames;
			bool exportSkins;
			float boneSize;

			IImported^ imported;

			char* cDest;
			FbxManager* pSdkManager;
			FbxScene* pScene;
			FbxExporter* pExporter;
			FbxArray<FbxSurfacePhong*>* pMaterials;
			FbxArray<FbxFileTexture*>* pTextures;
			FbxArray<FbxNode*>* pMeshNodes;

			~Exporter();
			void Fbx::Exporter::LinkTexture(ImportedMaterial^ mat, int attIndex, FbxFileTexture* pTexture, FbxProperty& prop);
			void SetJointsNode(FbxNode* pNode, HashSet<String^>^ boneNames, bool allBones);

			Exporter(String^ path, IImported^ imported, bool allFrames, bool allBones, bool skins, float boneSize, bool compatibility, bool normals);
			HashSet<String^>^ SearchHierarchy();
			void SearchHierarchy(ImportedFrame^ frame, HashSet<String^>^ exportFrames);
			void SetJointsFromImportedMeshes(bool allBones);
			void ExportFrame(FbxNode* pParentNode, ImportedFrame^ frame);
			void ExportMesh(FbxNode* pFrameNode, ImportedMesh^ meshList, bool normals);
			FbxFileTexture* ExportTexture(ImportedTexture^ matTex, FbxMesh* pMesh);
			void ExportAnimations(bool EulerFilter, float filterValue, bool flatInbetween);
			void ExportKeyframedAnimation(ImportedKeyframedAnimation^ parser, FbxString& kTakeName, FbxAnimCurveFilterUnroll* EulerFilter, float filterPrecision, bool flatInbetween);
			void ExportMorphs(IImported^ imported, bool morphMask, bool flatInbetween);
		};

		static char* StringToCharArray(String^ s);
		static void Init(FbxManager** pSdkManager, FbxScene** pScene);
	};
}
