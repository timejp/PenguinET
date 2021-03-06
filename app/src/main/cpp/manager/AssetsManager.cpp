//
// Created by 윤중현 on 2017. 3. 3..
//

#include "AssetsManager.h"

AssetsManager *AssetsManager::_assetsManager = NULL;

AssetsManager& AssetsManager::getInstance()
{
    if (_assetsManager == NULL) {
        LOGE("AssetsManager is not created! Please createAssetsManager first\n");
    }
    return *_assetsManager;
}

void AssetsManager::createAssetsManager(JNIEnv * env, jobject assetManager)
{
    if (_assetsManager == NULL) {
        _assetsManager = new AssetsManager();
        if (_assetsManager->_aAssetManager == NULL) {
            _assetsManager->_aAssetManager = AAssetManager_fromJava(env, assetManager);
            if (_assetsManager->_aAssetManager != NULL) {
                LOGI("AssetsManager is created!\n");
            }
            else {
                LOGE("AssetsManager is not created!\n");
            }
        }
    }
}

AssetsManager::AssetsManager()
{

}

AssetsManager::~AssetsManager()
{
    delete _aAssetManager;
}

void AssetsManager::destroy()
{
    AssetsManager *assetsManager = &getInstance();

    if (assetsManager != NULL) {
        delete assetsManager;
    }
}

char* AssetsManager::readFile(const char* filename)
{
    AAsset *asset = AAssetManager_open(AssetsManager::_aAssetManager, filename,
                                       AASSET_MODE_UNKNOWN);
    if (asset == NULL)
        return NULL;

    int fileSize = AAsset_getLength(asset);

    if (fileSize == 0)
        return NULL;

    char *buf = (char *) malloc(sizeof(char) * fileSize);
    AAsset_read(asset, buf, fileSize);

    AAsset_close(asset);

    // 읽어올때 마지막에 이상한 값이 추가가 되는 경우가 있어 이를 방지하기 위해 맨 마지막의 '}'까지만 잘라버린다.
    for (int i=fileSize;i>0;i--) {
        if (buf[i] == '}') {
            buf[i + 1] = '\0';
            break;
        }
    }
    LOGI("AssetsManager : %s is loaded. content -> %s\n", filename, buf);

    return buf;
}