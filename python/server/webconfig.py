__author__ = 'soulcry'
import os

class ConfigClass(object):
    # Flask settings
    SECRET_KEY =              '8gR1fnj2VHCXgzVTMPN5WnRg2sxeEGll2XcxQ78u'
    SQLALCHEMY_DATABASE_URI = 'sqlite:///login_app.sqlite'
    CSRF_ENABLED = True
    SQLALCHEMY_TRACK_MODIFICATIONS = False

    # Flask-Mail settings
    MAIL_USERNAME =           'tremppi.server@gmail.com'
    MAIL_PASSWORD =           'TREMPPIMAIL2016'
    MAIL_DEFAULT_SENDER =     '"TREMPPI" <tremmpi.server@gmail.com>'
    MAIL_SERVER =             'smtp.gmail.com'
    MAIL_PORT =               587
    MAIL_USE_SSL =            False
    MAIL_USE_TLS =            True

    # Flask-User settings
    USER_APP_NAME        = "TREMPPI"    # Used by email templates

    # Recaptcha setting
    RECAPTCHA_PARAMETERS = {'hl': 'en'}
    RECAPTCHA_DATA_ATTRS = {'theme': 'dark'}
    RECAPTCHA_PUBLIC_KEY = '6LfCFB4TAAAAAPxyAxDkH_X5_tNQJdjh6sND1O2Z'
    RECAPTCHA_PRIVATE_KEY = '6LfCFB4TAAAAALhmfIOwGTW9Vq8uhYoSll0dsVw_'

    #For testing, this makes recaptcha always pass.
    #Site key: 6LeIxAcTAAAAAJcZVRqyHh71UMIEGNQ_MXjiZKhI
    #Secret key: 6LeIxAcTAAAAAGG-vFI1TnRWxMZNFuojJ4WifJWe